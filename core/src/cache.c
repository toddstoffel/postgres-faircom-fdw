/*-------------------------------------------------------------------------
 *
 * cache.c
 *		  Query result caching for FairCom FDW
 *
 * Implements in-memory caching for query results with TTL-based expiration.
 * Focuses on COUNT(*) queries which are common in dashboards and reports.
 *
 * Copyright (c) 2026, FairCom Corporation. All rights reserved.
 * Proprietary and confidential.
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "faircom_fdw.h"

#include "utils/memutils.h"
#include "utils/timestamp.h"
#include "storage/lwlock.h"
#include "miscadmin.h"

/* Global cache - simple linked list for now */
static QueryCache *global_cache = NULL;
static MemoryContext cache_context = NULL;

/* Forward declarations */
static long TimestampDifferenceInSeconds(TimestampTz start_time, TimestampTz stop_time);

/*
 * Initialize the query cache
 */
void
faircom_cache_init(void)
{
	if (cache_context == NULL)
	{
		cache_context = AllocSetContextCreate(TopMemoryContext,
											  "FairCom Query Cache",
											  ALLOCSET_DEFAULT_SIZES);
	}

	if (global_cache == NULL)
	{
		MemoryContext oldcontext = MemoryContextSwitchTo(cache_context);

		global_cache = (QueryCache *) palloc0(sizeof(QueryCache));
		global_cache->entries = NULL;
		global_cache->entry_count = 0;
		global_cache->max_entries = FAIRCOM_CACHE_MAX_ENTRIES;

		MemoryContextSwitchTo(oldcontext);

		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Query cache initialized (max entries: %d, default TTL: %d seconds)",
			 FAIRCOM_CACHE_MAX_ENTRIES, FAIRCOM_CACHE_DEFAULT_TTL);
	}
}

/*
 * Cleanup the query cache
 */
void
faircom_cache_cleanup(void)
{
	if (cache_context != NULL)
	{
		MemoryContextDelete(cache_context);
		cache_context = NULL;
		global_cache = NULL;

		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Query cache cleaned up");
	}
}

/*
 * Generate cache key from table name and predicates
 */
static char *
generate_cache_key(const char *table_name, const char *predicates)
{
	StringInfoData key;

	initStringInfo(&key);
	appendStringInfo(&key, "COUNT:%s", table_name);

	if (predicates && predicates[0] != '\0')
		appendStringInfo(&key, ":%s", predicates);

	return key.data;
}

/*
 * Check if cache entry is expired
 */
static bool
is_cache_entry_expired(QueryCacheEntry *entry)
{
	TimestampTz now = GetCurrentTimestamp();
	long secs;
	int microsecs;

	TimestampDifference(entry->cached_at, now, &secs, &microsecs);

	return (secs >= entry->ttl_seconds);
}

/*
 * Get COUNT result from cache
 * Returns true if found and not expired, false otherwise
 */
bool
faircom_cache_get_count(const char *table_name, const char *predicates,
						int64 *result, int ttl_seconds)
{
	QueryCacheEntry *entry;
	char *cache_key;

	/* Ensure cache is initialized */
	if (global_cache == NULL)
		faircom_cache_init();

	/* Generate cache key */
	cache_key = generate_cache_key(table_name, predicates);

	/* Search for entry */
	for (entry = global_cache->entries; entry != NULL; entry = entry->next)
	{
		if (strcmp(entry->cache_key, cache_key) == 0)
		{
			/* Found entry - check if expired */
			if (is_cache_entry_expired(entry))
			{
				FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Cache entry expired for key '%s'", cache_key);
				pfree(cache_key);
				return false;
			}

			/* Cache hit! */
			*result = entry->result_value;
			FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Cache HIT for '%s' = %lld (age: %ld seconds)",
				 cache_key, (long long) *result,
				 (long) TimestampDifferenceInSeconds(entry->cached_at, GetCurrentTimestamp()));
			pfree(cache_key);
			return true;
		}
	}

	/* Cache miss */
	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Cache MISS for '%s'", cache_key);
	pfree(cache_key);
	return false;
}

/*
 * Store COUNT result in cache
 */
void
faircom_cache_set_count(const char *table_name, const char *predicates,
						int64 result, int ttl_seconds)
{
	QueryCacheEntry *entry;
	char *cache_key;
	MemoryContext oldcontext;

	/* Ensure cache is initialized */
	if (global_cache == NULL)
		faircom_cache_init();

	/* Check if cache is full - evict oldest entries if needed */
	if (global_cache->entry_count >= global_cache->max_entries)
	{
		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Cache full (%d entries), clearing expired entries",
			 global_cache->entry_count);
		faircom_cache_clear_expired();
	}

	/* Generate cache key */
	cache_key = generate_cache_key(table_name, predicates);

	/* Check if entry already exists - update it */
	for (entry = global_cache->entries; entry != NULL; entry = entry->next)
	{
		if (strcmp(entry->cache_key, cache_key) == 0)
		{
			/* Update existing entry */
			entry->result_value = result;
			entry->cached_at = GetCurrentTimestamp();
			entry->ttl_seconds = ttl_seconds;

			FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Cache UPDATED for '%s' = %lld (TTL: %d seconds)",
				 cache_key, (long long) result, ttl_seconds);
			pfree(cache_key);
			return;
		}
	}

	/* Create new entry */
	oldcontext = MemoryContextSwitchTo(cache_context);

	entry = (QueryCacheEntry *) palloc(sizeof(QueryCacheEntry));
	entry->cache_key = pstrdup(cache_key);
	entry->result_value = result;
	entry->cached_at = GetCurrentTimestamp();
	entry->ttl_seconds = ttl_seconds;
	entry->next = global_cache->entries;

	global_cache->entries = entry;
	global_cache->entry_count++;

	MemoryContextSwitchTo(oldcontext);

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Cache SET for '%s' = %lld (TTL: %d seconds, entries: %d)",
		 cache_key, (long long) result, ttl_seconds, global_cache->entry_count);

	pfree(cache_key);
}

/*
 * Invalidate all cache entries for a table
 */
void
faircom_cache_invalidate(const char *table_name)
{
	QueryCacheEntry *entry, *prev, *next;
	int removed_count = 0;

	if (global_cache == NULL)
		return;

	prev = NULL;
	entry = global_cache->entries;

	while (entry != NULL)
	{
		next = entry->next;

		/* Check if this entry is for the specified table */
		if (strstr(entry->cache_key, table_name) != NULL)
		{
			/* Remove this entry */
			if (prev == NULL)
				global_cache->entries = next;
			else
				prev->next = next;

			pfree(entry->cache_key);
			pfree(entry);
			global_cache->entry_count--;
			removed_count++;
		}
		else
		{
			prev = entry;
		}

		entry = next;
	}

	if (removed_count > 0)
	{
		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Invalidated %d cache entries for table '%s'",
			 removed_count, table_name);
	}
}

/*
 * Clear expired cache entries
 */
void
faircom_cache_clear_expired(void)
{
	QueryCacheEntry *entry, *prev, *next;
	int removed_count = 0;

	if (global_cache == NULL)
		return;

	prev = NULL;
	entry = global_cache->entries;

	while (entry != NULL)
	{
		next = entry->next;

		if (is_cache_entry_expired(entry))
		{
			/* Remove this entry */
			if (prev == NULL)
				global_cache->entries = next;
			else
				prev->next = next;

			pfree(entry->cache_key);
			pfree(entry);
			global_cache->entry_count--;
			removed_count++;
		}
		else
		{
			prev = entry;
		}

		entry = next;
	}

	if (removed_count > 0)
	{
		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Cleared %d expired cache entries (remaining: %d)",
			 removed_count, global_cache->entry_count);
	}
}

/*
 * Helper: Get seconds between two timestamps
 */
static long
TimestampDifferenceInSeconds(TimestampTz start_time, TimestampTz stop_time)
{
	long secs;
	int microsecs;

	TimestampDifference(start_time, stop_time, &secs, &microsecs);
	return secs;
}
