# FairCom Foreign Data Wrapper for PostgreSQL

MODULE_big = faircom_fdw
OBJS = src/faircom_fdw.o src/connection.o src/schema.o src/query.o src/types.o src/utils.o src/predicate.o src/cache.o src/plan_cache.o

EXTENSION = faircom_fdw  
DATA = faircom_fdw--1.0.sql

PG_CONFIG ?= pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)

UNAME_M := $(shell uname -m)
FAIRCOM_ARCH ?= $(if $(filter aarch64,$(UNAME_M)),arm64,$(if $(filter x86_64,$(UNAME_M)),amd64,$(UNAME_M)))
FAIRCOM_HOME ?= $(if $(wildcard $(CURDIR)/faircom_libs/$(FAIRCOM_ARCH)),$(CURDIR)/faircom_libs/$(FAIRCOM_ARCH),$(CURDIR)/faircom_libs)
FAIRCOM_INCLUDE ?= $(FAIRCOM_HOME)/include
FAIRCOM_LIB ?= $(FAIRCOM_HOME)/lib

WARN_CFLAGS ?= -Wall -Wextra

# Debug logging configuration:
# By default, DEBUG1/DEBUG2 statements are compiled out for zero overhead
# To enable debug logging, add -DFAIRCOM_DEBUG to PG_CPPFLAGS:
#   make PG_CPPFLAGS="$(PG_CPPFLAGS) -DFAIRCOM_DEBUG"
# Or set environment variable:
#   export PG_CPPFLAGS="$PG_CPPFLAGS -DFAIRCOM_DEBUG"

PG_CPPFLAGS = -I$(FAIRCOM_INCLUDE) -I$(FAIRCOM_INCLUDE)/unix/multithreaded/dynamic -Isrc -O2 $(WARN_CFLAGS)
SHLIB_LINK = -L$(FAIRCOM_LIB) -lmtclient -Wl,-rpath,$(FAIRCOM_LIB)

include $(PGXS)
