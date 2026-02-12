/*
 *      OFFICIAL NOTIFICATION: the following CONFIDENTIAL and PROPRIETARY
 * 	property legend shall not be removed from this source code module
 * 	for any reason.
 *
 *	This program is the CONFIDENTIAL and PROPRIETARY property
 *	of FairCom(R) Corporation. Any unauthorized use, reproduction or
 *	transfer of this computer program is strictly prohibited. The
 *      contents of this file may not be disclosed to third parties, copied or
 *      duplicated in any form, in whole or in part, without the prior written
 *      permission of the FairCom(R) Corporation.
 *
 *      Copyright (c) 1992 - 2024 FairCom Corporation.
 *	This is an unpublished work, and is subject to limited distribution and
 *	restricted disclosure only. ALL RIGHTS RESERVED.
 *
 *			RESTRICTED RIGHTS LEGEND
 *	Use, duplication, or disclosure by the Government is subject to
 *	restrictions set forth in subparagraph (c)(1)(ii) of the Rights in
 * 	Technical Data and Computer Software clause at DFARS 252.227-7013, and/or
 *      in similar or successor clauses in the FAR, DOD or NASA FAR Supplement.
 *      Unpublished rights reserved under the Copyright Laws of the United States.
 *	FairCom Corporation, 6300 West Sugar Creek Drive, Columbia, MO 65203.
 *
 *	c-treeACE(tm)	Version 13
 */

 /**
  * @file ctcbjson.h
  * @brief An wrapper API of the JSON functions used by RTG.
  * @author FairCom Corporation
  * @date 2021/06/28
  */

#ifndef _CTCBJSON_H_
#define _CTCBJSON_H_

#include "ctcboptn.h"
#include "ctjsonc.h"

#define JSON_OUT_OF_MEMORY (cpTEXT)"{\"status\":\"error\",\"error\":{\"code\":14,\"btrvCode\":101,\"message\":\"Out of memory\"}}" /* CTE_NO_MEMORY B_NO_OS_MEMORY_AVAIL */
#define JSON_INVALID_HANDLE (cpTEXT)"{\"status\":\"error\",\"error\":{\"code\":-32600,\"message\":\"Invalid handle\"}}"
#define JSON_INVALID_INPUT (cpTEXT)"{\"status\":\"error\",\"error\":{\"code\":-32600,\"message\":\"Invalid request\"}}"
#define JSON_SYNTAX_ERROR (cpTEXT)"{\"status\":\"error\",\"error\":{\"code\":-32700,\"message\":\"Parse error\"}}"

/** Create a new json_object
 *
 * @return - a new json_object of type json_type_object
 */
cbDLLexport json_object *rtgJsonNewObject(void);

/** Free a json_object
 *
 * @param obj - the json_object instance
 */
cbDLLexport void rtgJsonFreeObject(json_object *obj);

/** Create an object field and add it to a json_object
 *
 * @param obj - the json_object instance
 * @param key - the object field name
 * @return - a json_object or NULL
 */
json_object *rtgJsonAddObject(json_object *obj, const char *key);

/** Create an object field of type json_type_array and add it to a json_object
 *
 * @param obj - the json_object instance
 * @param key - the object field name
 * @return - a json_object of type json_type_array or NULL
 */
json_object *rtgJsonAddArray(json_object *obj, const char *key);

/** Create an object field and add it to the end of a json_object of type json_type_array
 *
 * @param obj the json_object instance
 * @return - a json_object or NULL
 */
json_object *rtgJsonAddArrayElement(json_object *obj);

/** Create an object field of type json_type_string and add it to a json_object
 *
 * @param obj - the json_object instance
 * @param key - the object field name
 * @param s - the string
 * @return - a json_object of type json_type_string
 */
cbDLLexport json_object *rtgJsonAddString(json_object *obj, const char *key, const char *s);

/** Create an object field of type json_type_boolean and add it to a json_object
 *
 * @param obj - the json_object instance
 * @param key - the object field name
 * @param b - the boolean value (1 or 0)
 * @return - a json_object of type json_type_boolean
 */
cbDLLexport json_object *rtgJsonAddBool(json_object *obj, const char *key, int32_t b);

/** Create an object field of type json_type_int and add it to a json_object
 *
 * @param obj - the json_object instance
 * @param key - the object field name
 * @param i - the integer
 * @return - a json_object of type json_type_boolean
 */
cbDLLexport json_object *rtgJsonAddInt(json_object *obj, const char *key, int32_t i);

/** Create an object field of type json_type_int and add it to a json_object
 *
 * @param obj - the json_object instance
 * @param key - the object field name
 * @param i - the integer
 * @return - a json_object of type json_type_boolean
 */
json_object *rtgJsonAddInt64(json_object *obj, const char *key, int64_t i);

/** Get the json_object associated with a given object field
 *
 * @param obj - the json_object instance
 * @param key - the object field name
 * @return - the json_object associated with the given field name
 */
cbDLLexport const json_object *rtgJsonGetObject(const json_object *obj, const char *key);

/** Get the json_object of type json_type_array associated with a given object field
 *
 * @param obj - the json_object instance
 * @param key - the object field name
 * @return - the json_object of type json_type_array associated with the given field name
 */
cbDLLexport const json_object *rtgJsonGetArray(const json_object *jobj, const char *key);

/** Get the element at specificed index of the array (a json_object of type json_type_array)
 *
 * @param obj - the json_object instance
 * @param idx - the index to get the element at
 * @return - the json_object at the specified index (or NULL)
 */
cbDLLexport const json_object *rtgJsonGetArrayElement(const json_object *jobj, size_t idx);

/** Get the value of a json_type_string json_object
 *
 * This returns true if the key is found, false in all other cases.
 *
 * @param obj - the json_object instance
 * @param key - the object field name
 * @param val - a pointer to the returning string
 * @return - whether or not the key exists
 */
cbDLLexport json_bool rtgJsonGetString(const json_object *jobj, const char *key, const char **val);

/** Get the value of a json_type_boolean json_object
 *
 * This returns true if the key is found, false in all other cases.
 *
 * @param obj - the json_object instance
 * @param key - the object field name
 * @param val - a pointer to the returning json_bool value
 * @return - whether or not the key exists
 */
cbDLLexport json_bool rtgJsonGetBool(const json_object *obj, const char *key, json_bool *val);

/** Get the value of a json_type_int json_object
 *
 * This returns true if the key is found, false in all other cases.
 *
 * @param obj - the json_object instance
 * @param key - the object field name
 * @param val - a pointer to the returning int32_t value
 * @return - whether or not the key exists
 */
cbDLLexport json_bool rtgJsonGetInt(const json_object *jobj, const char *key, int32_t *val);

/** Get the value of a json_type_int json_object
 *
 * This returns true if the key is found, false in all other cases.
 *
 * @param obj - the json_object instance
 * @param key - the object field name
 * @param val - a pointer to the returning int64_t value
 * @return - whether or not the key exists
 */
cbDLLexport json_bool rtgJsonGetInt64(const json_object *jobj, const char *key, int64_t *val);

/** Stringify object to json format
 *
 * @param obj - the json_object instance
 * @return - a string in JSON format
 */
cbDLLexport const char *rtgJsonObjectToString(json_object *obj);

/** Parse a string and return a non-NULL json_object if a valid JSON value is found.
 *
 * @param str - a string with any valid JSON expression
 * @param err - an error of type json_tokener_error
 * @return - the json_object associated with the given string
 */
cbDLLexport json_object *rtgJsonParse(const char *str, enum json_tokener_error *err);

#endif	/* _CTCBJSON_H_ */

/* end of ctcbjson.h */
