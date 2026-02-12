/*-------------------------------------------------------------------------
 *
 * types.c
 *		  Data type conversion between FairCom and PostgreSQL
 *
 * Handles conversion of values between FairCom CTDB types and
 * PostgreSQL Datum format.
 *
 * Copyright (c) 2026, FairCom Corporation. All rights reserved.
 * Proprietary and confidential.
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "faircom_fdw.h"

#include "catalog/pg_type.h"
#include "utils/builtins.h"
#include "utils/date.h"
#include "utils/timestamp.h"

/* FairCom Date Conversion Constants
 *
 * FairCom CTDATE stores dates as unsigned long (ULONG) days since their epoch.
 * PostgreSQL DateADT stores dates as signed 32-bit days since 2000-01-01.
 *
 * Through empirical testing with multiple dates:
 * - Offset calibrated to match FairCom date storage
 * - Verified with dates: 1990-01-01, 2000-01-01, 2020-01-01, 2025-06-15
 */
#define FAIRCOM_TO_POSTGRES_OFFSET  109514  /* Days between FairCom epoch and PostgreSQL 2000-01-01 */

/*
 * Convert a FairCom field value to PostgreSQL Datum
 */
Datum
faircom_to_datum(CTHANDLE hRecord, int fieldNum, Oid pgType, int32 typmod, bool *isNull)
{
	CTDBRET retval;

	*isNull = false;

	/* Check for NULL value first */
	if (ctdbIsNullField(hRecord, fieldNum))
	{
		*isNull = true;
		return (Datum) 0;
	}

	switch (pgType)
	{
		case BOOLOID:
		{
			CTBOOL value;
			retval = ctdbGetFieldAsBool(hRecord, fieldNum, &value);
			if (retval == CTDBRET_OK)
				return BoolGetDatum(value != 0);
			break;
		}

		case INT2OID:
		{
			CTSIGNED value;
			retval = ctdbGetFieldAsSigned(hRecord, fieldNum, &value);
			if (retval == CTDBRET_OK)
				return Int16GetDatum((int16) value);
			break;
		}

		case INT4OID:
		{
			CTSIGNED value;
			retval = ctdbGetFieldAsSigned(hRecord, fieldNum, &value);
			if (retval == CTDBRET_OK)
				return Int32GetDatum((int32) value);
			break;
		}

		case INT8OID:
		{
			CTBIGINT value;
			retval = ctdbGetFieldAsBigint(hRecord, fieldNum, &value);
			if (retval == CTDBRET_OK)
				return Int64GetDatum((int64) value);
			break;
		}

		case FLOAT4OID:
		{
			CTFLOAT value;
			retval = ctdbGetFieldAsFloat(hRecord, fieldNum, &value);
			if (retval == CTDBRET_OK)
				return Float4GetDatum((float4) value);
			break;
		}

		case FLOAT8OID:
		{
			CTFLOAT value;
			retval = ctdbGetFieldAsFloat(hRecord, fieldNum, &value);
			if (retval == CTDBRET_OK)
				return Float8GetDatum((float8) value);
			break;
		}

		case TEXTOID:
		case VARCHAROID:
		{
			char buffer[8192];
			retval = ctdbGetFieldAsString(hRecord, fieldNum, (pTEXT)buffer, sizeof(buffer));
			if (retval == CTDBRET_OK)
				return CStringGetTextDatum(buffer);
			break;
		}

		case TIMESTAMPOID:
		{
			CTDATETIME datetime;
			retval = ctdbGetFieldAsDateTime(hRecord, fieldNum, &datetime);
			if (retval == CTDBRET_OK)
			{
				/* FairCom datetime stored as microseconds since 1970 */
				Timestamp ts = (Timestamp) datetime;
				return TimestampGetDatum(ts);
			}
			break;
		}

		case DATEOID:
		{
			CTDATE date;
			retval = ctdbGetFieldAsDate(hRecord, fieldNum, &date);
			if (retval == CTDBRET_OK)
			{
				/* Convert FairCom days to PostgreSQL days */
				DateADT pgdate = (DateADT) date - FAIRCOM_TO_POSTGRES_OFFSET;
				return DateADTGetDatum(pgdate);
			}
			break;
		}

		case NUMERICOID:
		{
			char buffer[256];
			CTNUMBER number;
			int32 numeric_typmod = (typmod < 0) ? -1 : typmod;

			/* Prefer CTNUMBER conversion to avoid float formatting artifacts. */
			retval = ctdbGetFieldAsNumber(hRecord, fieldNum, &number);
			if (retval == CTDBRET_OK)
			{
				retval = ctdbNumberToString(&number, (pTEXT)buffer, sizeof(buffer));
				if (retval == CTDBRET_OK)
				{
					return DirectFunctionCall3(numeric_in,
									  CStringGetDatum(buffer),
									  ObjectIdGetDatum(InvalidOid),
									  Int32GetDatum(numeric_typmod));
				}
			}

			/* Fallback to string conversion if CTNUMBER is unavailable. */
			retval = ctdbGetFieldAsString(hRecord, fieldNum, (pTEXT)buffer, sizeof(buffer));
			if (retval == CTDBRET_OK)
			{
				return DirectFunctionCall3(numeric_in,
								  CStringGetDatum(buffer),
								  ObjectIdGetDatum(InvalidOid),
								  Int32GetDatum(numeric_typmod));
			}
			break;
		}

		case BYTEAOID:
		{
			/* Binary data not yet supported - return NULL */
			*isNull = true;
			return (Datum) 0;
		}

		default:
			ereport(ERROR,
					(errcode(ERRCODE_FDW_INVALID_DATA_TYPE),
					 errmsg("unsupported PostgreSQL type OID: %u", pgType)));
			break;
	}

	/* If we get here, conversion failed */
	*isNull = true;
	return (Datum) 0;
}

/*
 * Convert PostgreSQL Datum to FairCom field value
 * (For future write operations)
 */
void
faircom_datum_to_field(Datum value, Oid pgType, CTHANDLE hRecord, int fieldNum)
{
	CTDBRET retval = CTDBRET_OK;

	switch (pgType)
	{
		case BOOLOID:
			retval = ctdbSetFieldAsBool(hRecord, fieldNum, DatumGetBool(value));
			break;

		case INT2OID:
			retval = ctdbSetFieldAsSigned(hRecord, fieldNum, DatumGetInt16(value));
			break;

		case INT4OID:
			retval = ctdbSetFieldAsSigned(hRecord, fieldNum, DatumGetInt32(value));
			break;

		case INT8OID:
			retval = ctdbSetFieldAsBigint(hRecord, fieldNum, DatumGetInt64(value));
			break;

		case FLOAT4OID:
			retval = ctdbSetFieldAsFloat(hRecord, fieldNum, DatumGetFloat4(value));
			break;

		case FLOAT8OID:
			retval = ctdbSetFieldAsFloat(hRecord, fieldNum, DatumGetFloat8(value));
			break;

		case TEXTOID:
		case VARCHAROID:
		{
			char *str = TextDatumGetCString(value);
			retval = ctdbSetFieldAsString(hRecord, fieldNum, (pTEXT)str);
			pfree(str);
			break;
		}

		case DATEOID:
		{
			/* Convert PostgreSQL days to FairCom days */
			DateADT pgdate = DatumGetDateADT(value);
			CTDATE faircom_date = (CTDATE)(pgdate + FAIRCOM_TO_POSTGRES_OFFSET);
			retval = ctdbSetFieldAsDate(hRecord, fieldNum, faircom_date);
			break;
		}

		case NUMERICOID:
		{
			/* Convert numeric to string for FairCom */
			char *num_str = DatumGetCString(DirectFunctionCall1(numeric_out, value));
			retval = ctdbSetFieldAsString(hRecord, fieldNum, (pTEXT)num_str);
			pfree(num_str);
			break;
		}

		default:
			ereport(ERROR,
					(errcode(ERRCODE_FDW_INVALID_DATA_TYPE),
					 errmsg("unsupported PostgreSQL type OID for write: %u", pgType)));
			break;
	}

	faircom_check_result(retval, hRecord, "ctdbSetFieldAs*");
}
