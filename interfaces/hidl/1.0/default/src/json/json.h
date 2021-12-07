/*
 * Copyright (c) 2021 anqi.huang@outlook.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _JSON_H_
#define _JSON_H_

typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;

/* Json Types: */
#define Json_False 0
#define Json_True 1
#define Json_NULL 2
#define Json_Int 3
#define Json_Double 4
#define Json_String 5
#define Json_Array 6
#define Json_Object 7

#define Json_IsReference 256

/* The Json structure: */
typedef struct Json {
    /*
     * next/prev allow you to walk array/object chains.
     * Alternatively, use GetArraySize/GetArrayItem/GetObjectItem
     */
    struct Json *next, *prev;

    /*
     * An array or object item will have a child pointer
     * pointing to a chain of the items in the array/object.
     */
    struct Json *child;

    /*
     * The type of the item, as above.
     */
    int type;

    /*
     * The item's string, if type==Json_String
     */
    char *valuestring;

    /*
     * The item's number, if type==Json_Number
	 */
    uint64 valueint;

    /*
     * The item's number, if type==Json_Number
     */
    double valuedouble;

    /*
     * sign of valueint, 1(unsigned), -1(signed)
     */
    int sign;

    /*
     * The item's name string, if this item is the child of,
     * or is in the list of subitems of an object.
     */
    char *string;
} Json;

typedef struct Json_Hooks {
    void *(*malloc_fn)(size_t sz);

    void (*free_fn)(void *ptr);
} Json_Hooks;

/*
 * Supply malloc, realloc and free functions to Json
 */
extern void Json_InitHooks(Json_Hooks *hooks);

/*
 * Supply a block of JSON, and this returns
 * a Json object you can interrogate.
 * Call Json_Delete when finished.
 */
extern Json *Json_Parse(const char *value);

/*
 * Render a Json entity to text for transfer/storage.
 * Free the char* when finished.
 */
extern char *Json_Print(Json *item);

/*
 * Render a Json entity to text for transfer/storage
 * without any formatting. Free the char* when finished.
 */
extern char *Json_PrintUnformatted(Json *item);

/*
 * Delete a Json entity and all subentities.
 */
extern void Json_Delete(Json *c);

/*
 * Returns the number of items in an array (or object).
 */
extern int Json_GetArraySize(Json *array);

/*
 * Retrieve item number "item" from array "array".
 * Returns NULL if unsuccessful.
 */
extern Json *Json_GetArrayItem(Json *array, int item);

/*
 * Get item "string" from object.
 * Case insensitive.
 */
extern Json *Json_GetObjectItem(Json *object, const char *string);

/*
 * For analysing failed parses.
 * This returns a pointer to the parse error.
 * You'll probably need to look a few chars back to make sense of it.
 * Defined when Json_Parse() returns 0. 0 when Json_Parse() succeeds.
 */
extern const char *Json_GetErrorPtr();

/*
 * These calls create a Json item of the appropriate type.
 */
extern Json *Json_CreateNull();

extern Json *Json_CreateTrue();

extern Json *Json_CreateFalse();

extern Json *Json_CreateBool(int b);

extern Json *Json_CreateDouble(double num, int sign);

extern Json *Json_CreateInt(uint64 num, int sign);

extern Json *Json_CreateString(const char *string);

extern Json *Json_CreateArray();

extern Json *Json_CreateObject();

/*
 * These utilities create an Array of count items.
 */
extern Json *Json_CreateIntArray(int *numbers, int sign, int count);

extern Json *Json_CreateFloatArray(float *numbers, int count);

extern Json *Json_CreateDoubleArray(double *numbers, int count);

extern Json *Json_CreateStringArray(const char **strings, int count);

/*
 * Append item to the specified array/object.
 */
extern void Json_AddItemToArray(Json *array, Json *item);

extern void Json_AddItemToArrayHead(Json *array, Json *item);

extern void Json_AddItemToObject(Json *object, const char *string, Json *item);

/*
 * Append reference to item to the specified array/object.
 * Use this when you want to add an existing Json to a new Json,
 * but don't want to corrupt your existing Json.
 */
extern void Json_AddItemReferenceToArray(Json *array, Json *item);

extern void Json_AddItemReferenceToObject(Json *object, const char *string, Json *item);

/*
 * Remove/Detatch items from Arrays/Objects.
 */
extern Json *Json_DetachItemFromArray(Json *array, int which);

extern void Json_DeleteItemFromArray(Json *array, int which);

extern Json *Json_DetachItemFromObject(Json *object, const char *string);

extern void Json_DeleteItemFromObject(Json *object, const char *string);

/*
 * Update array items.
 */
extern void Json_ReplaceItemInArray(Json *array, int which, Json *newitem);

extern void Json_ReplaceItemInObject(Json *object, const char *string, Json *newitem);

#define Json_AddNullToObject(object, name)        Json_AddItemToObject(object, name, Json_CreateNull())
#define Json_AddTrueToObject(object, name)        Json_AddItemToObject(object, name, Json_CreateTrue())
#define Json_AddFalseToObject(object, name)        Json_AddItemToObject(object, name, Json_CreateFalse())
#define Json_AddNumberToObject(object, name, n)    Json_AddItemToObject(object, name, Json_CreateNumber(n))
#define Json_AddStringToObject(object, name, s)    Json_AddItemToObject(object, name, Json_CreateString(s))

#endif
