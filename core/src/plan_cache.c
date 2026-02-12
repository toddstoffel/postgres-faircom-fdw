/*-------------------------------------------------------------------------
 *
 * plan_cache.c
 *		Query plan caching for FairCom FDW
 *
 * Caches table metadata, column information, and index details to avoid
 * repeated FairCom metadata calls during query planning.
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

/* Global plan cache and memory context */
static PlanCache *global_plan_cache = NULL;
static MemoryContext plan_cache_context = NULL;

/* Forward declaration */
static long TimestampDifferenceInSeconds(TimestampTz start, TimestampTz end);

/*
 * faircom_plan_cache_init
 *		Initialize the plan cache in TopMemoryContext
 */
void
faircom_plan_cache_init(void)
{
	if (global_plan_cache != NULL)
		return;  /* Already initialized */

	/* Create long-lived memory context */
	plan_cache_context = AllocSetContextCreate(TopMemoryContext,
											   "FairCom Plan Cache",
											   ALLOCSET_DEFAULT_SIZES);

	/* Allocate cache structure */
	global_plan_cache = (PlanCache *) MemoryContextAllocZero(plan_cache_context,
															  sizeof(PlanCache));
	global_plan_cache->entries = NULL;
	global_plan_cache->entry_count = 0;
	global_plan_cache->max_entries = FAIRCOM_PLAN_CACHE_MAX_ENTRIES;

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom plan cache initialized (max entries: %d, TTL: %d seconds)",
		 FAIRCOM_PLAN_CACHE_MAX_ENTRIES, FAIRCOM_PLAN_CACHE_TTL);
}

/*
 * faircom_plan_cache_cleanup
 *		Clean up plan cache memory
 */
void
faircom_plan_cache_cleanup(void)
{
	if (plan_cache_context != NULL)
	{
		FAIRCOM_DEBUG_LOG(DEBUG1, "Cleaning up FairCom plan cache");
		MemoryContextDelete(plan_cache_context);
		plan_cache_context = NULL;
		global_plan_cache = NULL;
	}
}

/*
 * faircom_plan_cache_get
 *		Retrieve cached plan entry for a foreign table
 *		Returns NULL if not found or expired
 */
PlanCacheEntry *
faircom_plan_cache_get(Oid foreign_table_oid)
{
	PlanCacheEntry *entry;
	TimestampTz now;
	long age_seconds;

	if (global_plan_cache == NULL)
		faircom_plan_cache_init();

	FAIRCOM_DEBUG_LOG(DEBUG1, "Plan cache GET called: OID=%u, global=%p, entries=%p, entry_count=%d",
		 foreign_table_oid, global_plan_cache,
		 global_plan_cache ? global_plan_cache->entries : NULL,
		 global_plan_cache ? global_plan_cache->entry_count : 0);

	now = GetCurrentTimestamp();

	/* Search cache */
	for (entry = global_plan_cache->entries; entry != NULL; entry = entry->next)
	{
		if (entry->foreign_table_oid == foreign_table_oid)
		{
			/* Check if entry is still valid and not expired */
			if (!entry->is_valid)
			{
				FAIRCOM_DEBUG_LOG(DEBUG2, "Plan cache entry for OID %u is invalidated", foreign_table_oid);
				return NULL;
			}

			age_seconds = TimestampDifferenceInSeconds(entry->cached_at, now);

			if (age_seconds > FAIRCOM_PLAN_CACHE_TTL)
			{
				FAIRCOM_DEBUG_LOG(DEBUG2, "Plan cache entry for OID %u expired (age: %ld seconds)",
					 foreign_table_oid, age_seconds);
				return NULL;
			}

			/* Update last accessed time for LRU */
			entry->last_accessed = now;

			FAIRCOM_DEBUG_LOG(DEBUG1, "Plan cache HIT for OID %u (age: %ld seconds, %d columns, %d indexes)",
				 foreign_table_oid, age_seconds, entry->column_count,
				 entry->indexes ? 1 : 0);  /* Simplified count */
			return entry;
		}
	}

	FAIRCOM_DEBUG_LOG(DEBUG1, "Plan cache MISS for OID %u", foreign_table_oid);
	return NULL;
}

/*
 * faircom_plan_cache_set
 *		Store or update plan cache entry
 */
void
faircom_plan_cache_set(Oid foreign_table_oid,
					   const char *table_name,
					   int column_count,
					   char **column_names,
					   Oid *column_types,
					   PlanCacheIndexInfo *indexes,
					   CTUINT64 row_count)
{
	PlanCacheEntry *entry;
	PlanCacheEntry *existing = NULL;
	MemoryContext old_context;
	int i;

	if (global_plan_cache == NULL)
		faircom_plan_cache_init();

	/* Check if entry already exists */
	for (entry = global_plan_cache->entries; entry != NULL; entry = entry->next)
	{
		if (entry->foreign_table_oid == foreign_table_oid)
		{
			existing = entry;
			break;
		}
	}

	old_context = MemoryContextSwitchTo(plan_cache_context);

	if (existing)
	{
		/* Update existing entry */
		FAIRCOM_DEBUG_LOG(DEBUG2, "Updating existing plan cache entry for OID %u", foreign_table_oid);

		/* Free old column data */
		if (existing->column_names)
		{
			for (i = 0; i < existing->column_count; i++)
				pfree(existing->column_names[i]);
			pfree(existing->column_names);
		}
		if (existing->column_types)
			pfree(existing->column_types);
		if (existing->table_name)
			pfree(existing->table_name);

		entry = existing;
	}
	else
	{
		/* Create new entry */
		FAIRCOM_DEBUG_LOG(DEBUG2, "Creating new plan cache entry for OID %u", foreign_table_oid);

		/* Check cache size limit and evict LRU if full */
		if (global_plan_cache->entry_count >= global_plan_cache->max_entries)
		{
			PlanCacheEntry *lru_entry = NULL;
			PlanCacheEntry *lru_prev = NULL;
			PlanCacheEntry *curr, *prev;
			TimestampTz oldest_access = 0;

			FAIRCOM_DEBUG_LOG(DEBUG2, "Plan cache full (%d entries), evicting LRU entry",
				 global_plan_cache->max_entries);

			/* Find least recently used entry */
			for (curr = global_plan_cache->entries, prev = NULL;
				 curr != NULL;
				 prev = curr, curr = curr->next)
			{
				if (lru_entry == NULL || curr->last_accessed < oldest_access)
				{
					lru_entry = curr;
					lru_prev = prev;
					oldest_access = curr->last_accessed;
				}
			}

			/* Remove LRU entry from list */
			if (lru_entry)
			{
				if (lru_prev)
					lru_prev->next = lru_entry->next;
				else
					global_plan_cache->entries = lru_entry->next;

				/* Free entry memory */
				if (lru_entry->column_names)
				{
					for (i = 0; i < lru_entry->column_count; i++)
						pfree(lru_entry->column_names[i]);
					pfree(lru_entry->column_names);
				}
				if (lru_entry->column_types)
					pfree(lru_entry->column_types);
				if (lru_entry->table_name)
					pfree(lru_entry->table_name);
				pfree(lru_entry);

				global_plan_cache->entry_count--;

				FAIRCOM_DEBUG_LOG(DEBUG1, "Evicted LRU plan cache entry for OID %u",
					 lru_entry->foreign_table_oid);
			}
		}

		entry = (PlanCacheEntry *) palloc0(sizeof(PlanCacheEntry));
		entry->foreign_table_oid = foreign_table_oid;

		/* Add to head of list */
		entry->next = global_plan_cache->entries;
		global_plan_cache->entries = entry;
		global_plan_cache->entry_count++;
	}

	/* Copy table name */
	entry->table_name = pstrdup(table_name);

	/* Copy column information (only if provided) */
	entry->column_count = column_count;
	if (column_count > 0 && column_names != NULL && column_types != NULL)
	{
		entry->column_names = (char **) palloc(column_count * sizeof(char *));
		entry->column_types = (Oid *) palloc(column_count * sizeof(Oid));

		for (i = 0; i < column_count; i++)
		{
			entry->column_names[i] = pstrdup(column_names[i]);
			entry->column_types[i] = column_types[i];
		}
	}
	else
	{
		entry->column_names = NULL;
		entry->column_types = NULL;
	}

	/* Copy index information (shallow copy - caller owns index structures) */
	entry->indexes = indexes;

	/* Store row count */
	entry->row_count = row_count;

	/* Set metadata */
	entry->cached_at = GetCurrentTimestamp();
	entry->last_accessed = entry->cached_at;  /* Initialize last_accessed */
	entry->is_valid = true;

	FAIRCOM_DEBUG_LOG(DEBUG1, "About to cache entry: OID=%u table=%s valid=%d context=%p global=%p",
		 foreign_table_oid, table_name, entry->is_valid, plan_cache_context, global_plan_cache);
	FAIRCOM_DEBUG_LOG(DEBUG1, "Entry address=%p entry->next=%p global->entries=%p",
		 entry, entry->next, global_plan_cache->entries);

	MemoryContextSwitchTo(old_context);

	FAIRCOM_DEBUG_LOG(DEBUG1, "Plan cache SET for OID %u: table=%s, columns=%d, rows=%lu",
		 foreign_table_oid, table_name, column_count, (unsigned long)row_count);
}

/*
 * faircom_plan_cache_invalidate
 *		Mark a plan cache entry as invalid
 */
void
faircom_plan_cache_invalidate(Oid foreign_table_oid)
{
	PlanCacheEntry *entry;

	if (global_plan_cache == NULL)
		return;

	for (entry = global_plan_cache->entries; entry != NULL; entry = entry->next)
	{
		if (entry->foreign_table_oid == foreign_table_oid)
		{
			entry->is_valid = false;
			FAIRCOM_DEBUG_LOG(DEBUG1, "Plan cache invalidated for OID %u", foreign_table_oid);
			return;
		}
	}
}

/*
 * faircom_plan_cache_clear_all
 *		Clear all plan cache entries
 */
void
faircom_plan_cache_clear_all(void)
{
	if (global_plan_cache == NULL)
		return;

	FAIRCOM_DEBUG_LOG(DEBUG1, "Clearing all plan cache entries (%d total)",
		 global_plan_cache->entry_count);

	/* Just reset - memory will be freed when context is deleted */
	global_plan_cache->entries = NULL;
	global_plan_cache->entry_count = 0;

	/* Reset the memory context to free memory */
	if (plan_cache_context != NULL)
	{
		MemoryContextReset(plan_cache_context);

		/* Re-allocate cache structure */
		global_plan_cache = (PlanCache *) MemoryContextAllocZero(plan_cache_context,
																  sizeof(PlanCache));
		global_plan_cache->entries = NULL;
		global_plan_cache->entry_count = 0;
		global_plan_cache->max_entries = FAIRCOM_PLAN_CACHE_MAX_ENTRIES;
	}
}

/*
 * TimestampDifferenceInSeconds
 *		Calculate difference between timestamps in seconds
 */
static long
TimestampDifferenceInSeconds(TimestampTz start, TimestampTz end)
{
	long secs;
	int microsecs;

	TimestampDifference(start, end, &secs, &microsecs);
	return secs;
}
