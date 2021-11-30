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

#ifndef JSON_OBJECT_H_
#define JSON_OBJECT_H_

#include <stdio.h>
#include <stddef.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <string>
#include <map>
#include <list>

#ifdef __linux__
#include <malloc.h>
#endif

#ifdef __APPLE__
#include <sys/malloc.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "json.h"

#ifdef __cplusplus
}
#endif

class JsonObject {
public:     // method of ordinary json object or json array
    JsonObject();

    JsonObject(const std::string &strJson);

    JsonObject(const JsonObject *pJsonObject);

    JsonObject(const JsonObject &oJsonObject);

    virtual ~JsonObject();

    JsonObject &operator=(const JsonObject &oJsonObject);

    bool operator==(const JsonObject &oJsonObject) const;

    bool Parse(const std::string &strJson);

    void Clear();

    bool IsEmpty() const;

    bool IsArray() const;

    std::string ToString() const;

    std::string ToFormattedString() const;

    const std::string &GetErrMsg() const {
        return (m_strErrMsg);
    }

public:     // method of ordinary json object
    bool AddEmptySubObject(const std::string &strKey);

    bool AddEmptySubArray(const std::string &strKey);

    bool GetKey(std::string &strKey);

    void ResetTraversing();

    JsonObject &operator[](const std::string &strKey);

    std::string operator()(const std::string &strKey) const;

    bool Get(const std::string &strKey, JsonObject &oJsonObject) const;

    bool Get(const std::string &strKey, std::string &strValue) const;

    bool Get(const std::string &strKey, int32 &iValue) const;

    bool Get(const std::string &strKey, uint32 &uiValue) const;

    bool Get(const std::string &strKey, int64 &llValue) const;

    bool Get(const std::string &strKey, uint64 &ullValue) const;

    bool Get(const std::string &strKey, bool &bValue) const;

    bool Get(const std::string &strKey, float &fValue) const;

    bool Get(const std::string &strKey, double &dValue) const;

    bool IsNull(const std::string &strKey) const;

    bool Add(const std::string &strKey, const JsonObject &oJsonObject);

    bool Add(const std::string &strKey, const std::string &strValue);

    bool Add(const std::string &strKey, int32 iValue);

    bool Add(const std::string &strKey, uint32 uiValue);

    bool Add(const std::string &strKey, int64 llValue);

    bool Add(const std::string &strKey, uint64 ullValue);

    bool Add(const std::string &strKey, bool bValue, bool bValueAgain);

    bool Add(const std::string &strKey, float fValue);

    bool Add(const std::string &strKey, double dValue);

    bool AddNull(const std::string &strKey);    // add null like this:   "key":null
    bool Delete(const std::string &strKey);

    bool Replace(const std::string &strKey, const JsonObject &oJsonObject);

    bool Replace(const std::string &strKey, const std::string &strValue);

    bool Replace(const std::string &strKey, int32 iValue);

    bool Replace(const std::string &strKey, uint32 uiValue);

    bool Replace(const std::string &strKey, int64 llValue);

    bool Replace(const std::string &strKey, uint64 ullValue);

    bool Replace(const std::string &strKey, bool bValue, bool bValueAgain);

    bool Replace(const std::string &strKey, float fValue);

    bool Replace(const std::string &strKey, double dValue);

    bool ReplaceWithNull(const std::string &strKey);    // replace value with null

public:     // method of json array
    int GetArraySize();

    JsonObject &operator[](unsigned int uiWhich);

    std::string operator()(unsigned int uiWhich) const;

    bool Get(int iWhich, JsonObject &oJsonObject) const;

    bool Get(int iWhich, std::string &strValue) const;

    bool Get(int iWhich, int32 &iValue) const;

    bool Get(int iWhich, uint32 &uiValue) const;

    bool Get(int iWhich, int64 &llValue) const;

    bool Get(int iWhich, uint64 &ullValue) const;

    bool Get(int iWhich, bool &bValue) const;

    bool Get(int iWhich, float &fValue) const;

    bool Get(int iWhich, double &dValue) const;

    bool IsNull(int iWhich) const;

    bool Add(const JsonObject &oJsonObject);

    bool Add(const std::string &strValue);

    bool Add(int32 iValue);

    bool Add(uint32 uiValue);

    bool Add(int64 llValue);

    bool Add(uint64 ullValue);

    bool Add(int iAnywhere, bool bValue);

    bool Add(float fValue);

    bool Add(double dValue);

    bool AddNull();   // add a null value
    bool AddAsFirst(const JsonObject &oJsonObject);

    bool AddAsFirst(const std::string &strValue);

    bool AddAsFirst(int32 iValue);

    bool AddAsFirst(uint32 uiValue);

    bool AddAsFirst(int64 llValue);

    bool AddAsFirst(uint64 ullValue);

    bool AddAsFirst(int iAnywhere, bool bValue);

    bool AddAsFirst(float fValue);

    bool AddAsFirst(double dValue);

    bool AddNullAsFirst();     // add a null value
    bool Delete(int iWhich);

    bool Replace(int iWhich, const JsonObject &oJsonObject);

    bool Replace(int iWhich, const std::string &strValue);

    bool Replace(int iWhich, int32 iValue);

    bool Replace(int iWhich, uint32 uiValue);

    bool Replace(int iWhich, int64 llValue);

    bool Replace(int iWhich, uint64 ullValue);

    bool Replace(int iWhich, bool bValue, bool bValueAgain);

    bool Replace(int iWhich, float fValue);

    bool Replace(int iWhich, double dValue);

    bool ReplaceWithNull(int iWhich);      // replace with a null value

private:
    JsonObject(Json *pJsonData);

private:
    Json *m_pJsonData;
    Json *m_pExternJsonDataRef;
    std::string m_strErrMsg;
    std::map<unsigned int, JsonObject *> m_mapJsonArrayRef;
    std::map<std::string, JsonObject *> m_mapJsonObjectRef;
    std::list <std::string> m_listKeys;
    std::list<std::string>::const_iterator m_itKey;
};

#endif /* JsonHELPER_H_ */