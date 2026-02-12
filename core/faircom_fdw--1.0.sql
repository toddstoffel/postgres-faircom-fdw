-- FairCom Foreign Data Wrapper Extension Definition
-- Version 1.0

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION faircom_fdw" to load this file. \quit

-- Register the foreign data wrapper handler
CREATE FUNCTION faircom_fdw_handler()
RETURNS fdw_handler
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

-- Register the validator function
CREATE FUNCTION faircom_fdw_validator(text[], oid)
RETURNS void
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

-- Create the foreign data wrapper
CREATE FOREIGN DATA WRAPPER faircom_fdw
  HANDLER faircom_fdw_handler
  VALIDATOR faircom_fdw_validator;

-- Grant usage to public (can be restricted as needed)
COMMENT ON FOREIGN DATA WRAPPER faircom_fdw IS
  'Foreign data wrapper for accessing FairCom databases using CTDB API';
