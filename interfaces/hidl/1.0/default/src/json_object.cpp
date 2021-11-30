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

#include "json_object.h"

JsonObject::JsonObject() : m_pJsonData(NULL), m_pExternJsonDataRef(NULL) {
    // m_pJsonData = Json_CreateObject();
}

JsonObject::JsonObject(const std::string &strJson) : m_pJsonData(NULL), m_pExternJsonDataRef(NULL) {
    Parse(strJson);
}

JsonObject::JsonObject(const JsonObject *pJsonObject) : m_pJsonData(NULL), m_pExternJsonDataRef(NULL) {
    if (pJsonObject) {
        Parse(pJsonObject->ToString());
    }
}

JsonObject::JsonObject(const JsonObject &oJsonObject) : m_pJsonData(NULL), m_pExternJsonDataRef(NULL) {
    Parse(oJsonObject.ToString());
}

JsonObject::~JsonObject() {
    Clear();
}

JsonObject &JsonObject::operator=(const JsonObject &oJsonObject) {
    Parse(oJsonObject.ToString().c_str());
    return (*this);
}

bool JsonObject::operator==(const JsonObject &oJsonObject) const {
    return (this->ToString() == oJsonObject.ToString());
}

bool JsonObject::AddEmptySubObject(const std::string &strKey) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateObject();
    if (pJsonStruct == NULL) {
        m_strErrMsg = std::string("create sub empty object error!");
        return (false);
    }

    Json_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    m_listKeys.clear();

    return (true);
}

bool JsonObject::AddEmptySubArray(const std::string &strKey) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateArray();
    if (pJsonStruct == NULL) {
        m_strErrMsg = std::string("create sub empty array error!");
        return (false);
    }

    Json_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    m_listKeys.clear();
    return (true);
}

bool JsonObject::GetKey(std::string &strKey) {
    if (IsArray()) {
        return (false);
    }

    if (m_listKeys.size() == 0) {
        Json *pFocusData = NULL;
        if (m_pJsonData != NULL) {
            pFocusData = m_pJsonData;
        } else if (m_pExternJsonDataRef != NULL) {
            pFocusData = m_pExternJsonDataRef;
        } else {
            return (false);
        }

        Json *c = pFocusData->child;
        while (c) {
            m_listKeys.push_back(c->string);
            c = c->next;
        }
        m_itKey = m_listKeys.begin();
    }

    if (m_itKey == m_listKeys.end()) {
        strKey = "";
        m_itKey = m_listKeys.begin();
        return (false);
    } else {
        strKey = *m_itKey;
        ++m_itKey;
        return (true);
    }
}

void JsonObject::ResetTraversing() {
    m_itKey = m_listKeys.begin();
}

JsonObject &JsonObject::operator[](const std::string &strKey) {
    std::map<std::string, JsonObject *>::iterator iter;
    iter = m_mapJsonObjectRef.find(strKey);
    if (iter == m_mapJsonObjectRef.end()) {
        Json *pJsonStruct = NULL;
        if (m_pJsonData != NULL) {
            if (m_pJsonData->type == Json_Object) {
                pJsonStruct = Json_GetObjectItem(m_pJsonData, strKey.c_str());
            }
        } else if (m_pExternJsonDataRef != NULL) {
            if (m_pExternJsonDataRef->type == Json_Object) {
                pJsonStruct = Json_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
            }
        }

        if (pJsonStruct == NULL) {
            JsonObject *pJsonObject = new JsonObject();
            m_mapJsonObjectRef.insert(std::pair<std::string, JsonObject *>(strKey, pJsonObject));
            return (*pJsonObject);
        } else {
            JsonObject *pJsonObject = new JsonObject(pJsonStruct);
            m_mapJsonObjectRef.insert(std::pair<std::string, JsonObject *>(strKey, pJsonObject));
            return (*pJsonObject);
        }
    } else {
        return (*(iter->second));
    }
}

JsonObject &JsonObject::operator[](unsigned int uiWhich) {
    std::map<unsigned int, JsonObject *>::iterator iter;
    iter = m_mapJsonArrayRef.find(uiWhich);
    if (iter == m_mapJsonArrayRef.end()) {
        Json *pJsonStruct = NULL;
        if (m_pJsonData != NULL) {
            if (m_pJsonData->type == Json_Array) {
                pJsonStruct = Json_GetArrayItem(m_pJsonData, uiWhich);
            }
        } else if (m_pExternJsonDataRef != NULL) {
            if (m_pExternJsonDataRef->type == Json_Array) {
                pJsonStruct = Json_GetArrayItem(m_pExternJsonDataRef, uiWhich);
            }
        }
        if (pJsonStruct == NULL) {
            JsonObject *pJsonObject = new JsonObject();
            m_mapJsonArrayRef.insert(std::pair<unsigned int, JsonObject *>(uiWhich, pJsonObject));
            return (*pJsonObject);
        } else {
            JsonObject *pJsonObject = new JsonObject(pJsonStruct);
            m_mapJsonArrayRef.insert(std::pair<unsigned int, JsonObject *>(uiWhich, pJsonObject));
            return (*pJsonObject);
        }
    } else {
        return (*(iter->second));
    }
}

std::string JsonObject::operator()(const std::string &strKey) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pJsonData, strKey.c_str());
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
        }
    }

    if (pJsonStruct == NULL) {
        return (std::string(""));
    }

    if (pJsonStruct->type == Json_String) {
        return (pJsonStruct->valuestring);
    } else if (pJsonStruct->type == Json_Int) {
        char szNumber[128] = {0};
        if (pJsonStruct->sign == -1) {
            if ((int64) pJsonStruct->valueint <= (int64) INT_MAX && (int64) pJsonStruct->valueint >= (int64) INT_MIN) {
                snprintf(szNumber, sizeof(szNumber), "%d", (int32) pJsonStruct->valueint);
            } else {
                snprintf(szNumber, sizeof(szNumber), "%lld", (int64) pJsonStruct->valueint);
            }
        } else {
            if (pJsonStruct->valueint <= (uint64) UINT_MAX) {
                snprintf(szNumber, sizeof(szNumber), "%u", (uint32) pJsonStruct->valueint);
            } else {
                snprintf(szNumber, sizeof(szNumber), "%llu", pJsonStruct->valueint);
            }
        }
        return (std::string(szNumber));
    } else if (pJsonStruct->type == Json_Double) {
        char szNumber[128] = {0};
        if (fabs(pJsonStruct->valuedouble) < 1.0e-6 || fabs(pJsonStruct->valuedouble) > 1.0e9) {
            snprintf(szNumber, sizeof(szNumber), "%e", pJsonStruct->valuedouble);
        } else {
            snprintf(szNumber, sizeof(szNumber), "%f", pJsonStruct->valuedouble);
        }
    } else if (pJsonStruct->type == Json_False) {
        return (std::string("false"));
    } else if (pJsonStruct->type == Json_True) {
        return (std::string("true"));
    }

    return (std::string(""));
}

std::string JsonObject::operator()(unsigned int uiWhich) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pJsonData, uiWhich);
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pExternJsonDataRef, uiWhich);
        }
    }

    if (pJsonStruct == NULL) {
        return (std::string(""));
    }

    if (pJsonStruct->type == Json_String) {
        return (pJsonStruct->valuestring);
    } else if (pJsonStruct->type == Json_Int) {
        char szNumber[128] = {0};
        if (pJsonStruct->sign == -1) {
            if ((int64) pJsonStruct->valueint <= (int64) INT_MAX && (int64) pJsonStruct->valueint >= (int64) INT_MIN) {
                snprintf(szNumber, sizeof(szNumber), "%d", (int32) pJsonStruct->valueint);
            } else {
                snprintf(szNumber, sizeof(szNumber), "%lld", (int64) pJsonStruct->valueint);
            }
        } else {
            if (pJsonStruct->valueint <= (uint64) UINT_MAX) {
                snprintf(szNumber, sizeof(szNumber), "%u", (uint32) pJsonStruct->valueint);
            } else {
                snprintf(szNumber, sizeof(szNumber), "%llu", pJsonStruct->valueint);
            }
        }
        return (std::string(szNumber));
    } else if (pJsonStruct->type == Json_Double) {
        char szNumber[128] = {0};
        if (fabs(pJsonStruct->valuedouble) < 1.0e-6 || fabs(pJsonStruct->valuedouble) > 1.0e9) {
            snprintf(szNumber, sizeof(szNumber), "%e", pJsonStruct->valuedouble);
        } else {
            snprintf(szNumber, sizeof(szNumber), "%f", pJsonStruct->valuedouble);
        }
    } else if (pJsonStruct->type == Json_False) {
        return (std::string("false"));
    } else if (pJsonStruct->type == Json_True) {
        return (std::string("true"));
    }

    return (std::string(""));
}

bool JsonObject::Parse(const std::string &strJson) {
    Clear();
    m_pJsonData = Json_Parse(strJson.c_str());
    if (m_pJsonData == NULL) {
        m_strErrMsg = std::string("prase json string error at ") + Json_GetErrorPtr();
        return (false);
    }

    return (true);
}

void JsonObject::Clear() {
    m_pExternJsonDataRef = NULL;
    if (m_pJsonData != NULL) {
        Json_Delete(m_pJsonData);
        m_pJsonData = NULL;
    }

    for (std::map<unsigned int, JsonObject *>::iterator iter = m_mapJsonArrayRef.begin();
         iter != m_mapJsonArrayRef.end(); ++iter) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }
    }

    m_mapJsonArrayRef.clear();
    for (std::map<std::string, JsonObject *>::iterator iter = m_mapJsonObjectRef.begin();
         iter != m_mapJsonObjectRef.end(); ++iter) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }
    }

    m_mapJsonObjectRef.clear();
    m_listKeys.clear();
}

bool JsonObject::IsEmpty() const {
    if (m_pJsonData != NULL) {
        return (false);
    } else if (m_pExternJsonDataRef != NULL) {
        return (false);
    }

    return (true);
}

bool JsonObject::IsArray() const {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    }

    if (pFocusData == NULL) {
        return (false);
    }

    if (pFocusData->type == Json_Array) {
        return (true);
    } else {
        return (false);
    }
}

std::string JsonObject::ToString() const {
    char *pJsonString = NULL;
    std::string strJsonData = "";
    if (m_pJsonData != NULL) {
        pJsonString = Json_PrintUnformatted(m_pJsonData);
    } else if (m_pExternJsonDataRef != NULL) {
        pJsonString = Json_PrintUnformatted(m_pExternJsonDataRef);
    }

    if (pJsonString != NULL) {
        strJsonData = pJsonString;
        free(pJsonString);
    }

    return (strJsonData);
}

std::string JsonObject::ToFormattedString() const {
    char *pJsonString = NULL;
    std::string strJsonData = "";
    if (m_pJsonData != NULL) {
        pJsonString = Json_Print(m_pJsonData);
    } else if (m_pExternJsonDataRef != NULL) {
        pJsonString = Json_Print(m_pExternJsonDataRef);
    }

    if (pJsonString != NULL) {
        strJsonData = pJsonString;
        free(pJsonString);
    }

    return (strJsonData);
}


bool JsonObject::Get(const std::string &strKey, JsonObject &oJsonObject) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pJsonData, strKey.c_str());
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
        }
    }

    if (pJsonStruct == NULL) {
        return (false);
    }

    char *pJsonString = Json_Print(pJsonStruct);
    std::string strJsonData = pJsonString;
    free(pJsonString);
    if (oJsonObject.Parse(strJsonData)) {
        return (true);
    } else {
        return (false);
    }
}

bool JsonObject::Get(const std::string &strKey, std::string &strValue) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pJsonData, strKey.c_str());
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
        }
    }

    if (pJsonStruct == NULL) {
        return (false);
    }

    if (pJsonStruct->type != Json_String) {
        return (false);
    }

    strValue = pJsonStruct->valuestring;
    return (true);
}

bool JsonObject::Get(const std::string &strKey, int32 &iValue) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pJsonData, strKey.c_str());
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
        }
    }

    if (pJsonStruct == NULL) {
        return (false);
    }

    if (pJsonStruct->type == Json_Int) {
        iValue = (int32) (pJsonStruct->valueint);
        return (true);
    } else if (pJsonStruct->type == Json_Double) {
        iValue = (int32) (pJsonStruct->valuedouble);
        return (true);
    }

    return (false);
}

bool JsonObject::Get(const std::string &strKey, uint32 &uiValue) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pJsonData, strKey.c_str());
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
        }
    }

    if (pJsonStruct == NULL) {
        return (false);
    }

    if (pJsonStruct->type == Json_Int) {
        uiValue = (uint32) (pJsonStruct->valueint);
        return (true);
    } else if (pJsonStruct->type == Json_Double) {
        uiValue = (uint32) (pJsonStruct->valuedouble);
        return (true);
    }

    return (false);
}

bool JsonObject::Get(const std::string &strKey, int64 &llValue) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pJsonData, strKey.c_str());
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
        }
    }
    if (pJsonStruct == NULL) {
        return (false);
    }

    if (pJsonStruct->type == Json_Int) {
        llValue = (int64) (pJsonStruct->valueint);
        return (true);
    } else if (pJsonStruct->type == Json_Double) {
        llValue = (int64) (pJsonStruct->valuedouble);
        return (true);
    }

    return (false);
}

bool JsonObject::Get(const std::string &strKey, uint64 &ullValue) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pJsonData, strKey.c_str());
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
        }
    }

    if (pJsonStruct == NULL) {
        return (false);
    }

    if (pJsonStruct->type == Json_Int) {
        ullValue = (uint64) (pJsonStruct->valueint);
        return (true);
    } else if (pJsonStruct->type == Json_Double) {
        ullValue = (uint64) (pJsonStruct->valuedouble);
        return (true);
    }

    return (false);
}

bool JsonObject::Get(const std::string &strKey, bool &bValue) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pJsonData, strKey.c_str());
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
        }
    }

    if (pJsonStruct == NULL) {
        return (false);
    }

    if (pJsonStruct->type > Json_True) {
        return (false);
    }

    bValue = pJsonStruct->type;
    return (true);
}

bool JsonObject::Get(const std::string &strKey, float &fValue) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pJsonData, strKey.c_str());
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
        }
    }

    if (pJsonStruct == NULL) {
        return (false);
    }

    if (pJsonStruct->type == Json_Double) {
        fValue = (float) (pJsonStruct->valuedouble);
        return (true);
    } else if (pJsonStruct->type == Json_Int) {
        fValue = (float) (pJsonStruct->valueint);
        return (true);
    }

    return (false);
}

bool JsonObject::Get(const std::string &strKey, double &dValue) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pJsonData, strKey.c_str());
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
        }
    }

    if (pJsonStruct == NULL) {
        return (false);
    }

    if (pJsonStruct->type == Json_Double) {
        dValue = pJsonStruct->valuedouble;
        return (true);
    } else if (pJsonStruct->type == Json_Int) {
        dValue = (double) (pJsonStruct->valueint);
        return (true);
    }

    return (false);
}

bool JsonObject::IsNull(const std::string &strKey) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pJsonData, strKey.c_str());
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Object) {
            pJsonStruct = Json_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
        }
    }

    if (pJsonStruct == NULL) {
        return (false);
    }

    if (pJsonStruct->type != Json_NULL) {
        return (false);
    }

    return (true);
}

bool JsonObject::Add(const std::string &strKey, const JsonObject &oJsonObject) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_Parse(oJsonObject.ToString().c_str());
    if (pJsonStruct == NULL) {
        m_strErrMsg = std::string("prase json string error at ") + Json_GetErrorPtr();
        return (false);
    }

    Json_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }

    std::map<std::string, JsonObject *>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }
        m_mapJsonObjectRef.erase(iter);
    }

    m_listKeys.clear();
    return (true);
}

bool JsonObject::Add(const std::string &strKey, const std::string &strValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateString(strValue.c_str());
    if (pJsonStruct == NULL) {
        return (false);
    }

    Json_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }

    m_listKeys.clear();
    return (true);
}

bool JsonObject::Add(const std::string &strKey, int32 iValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt((uint64) iValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    Json_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }

    m_listKeys.clear();
    return (true);
}

bool JsonObject::Add(const std::string &strKey, uint32 uiValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt((uint64) uiValue, 1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    Json_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }

    m_listKeys.clear();
    return (true);
}

bool JsonObject::Add(const std::string &strKey, int64 llValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt((uint64) llValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    Json_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }

    m_listKeys.clear();
    return (true);
}

bool JsonObject::Add(const std::string &strKey, uint64 ullValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt(ullValue, 1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    Json_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }

    m_listKeys.clear();
    return (true);
}

bool JsonObject::Add(const std::string &strKey, bool bValue, bool bValueAgain) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateBool(bValue);
    if (pJsonStruct == NULL) {
        return (false);
    }

    Json_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }

    m_listKeys.clear();
    return (true);
}

bool JsonObject::Add(const std::string &strKey, float fValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateDouble((double) fValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    Json_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }

    m_listKeys.clear();
    return (true);
}

bool JsonObject::Add(const std::string &strKey, double dValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateDouble((double) dValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    Json_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }

    m_listKeys.clear();
    return (true);
}

bool JsonObject::AddNull(const std::string &strKey) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateObject();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateNull();
    if (pJsonStruct == NULL) {
        return (false);
    }

    Json_AddItemToObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }

    m_listKeys.clear();
    return (true);
}

bool JsonObject::Delete(const std::string &strKey) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json_DeleteItemFromObject(pFocusData, strKey.c_str());
    std::map<std::string, JsonObject *>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }
        m_mapJsonObjectRef.erase(iter);
    }

    m_listKeys.clear();
    return (true);
}

bool JsonObject::Replace(const std::string &strKey, const JsonObject &oJsonObject) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_Parse(oJsonObject.ToString().c_str());
    if (pJsonStruct == NULL) {
        m_strErrMsg = std::string("prase json string error at ") + Json_GetErrorPtr();
        return (false);
    }

    Json_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }

    std::map<std::string, JsonObject *>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }
        m_mapJsonObjectRef.erase(iter);
    }

    return (true);
}

bool JsonObject::Replace(const std::string &strKey, const std::string &strValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateString(strValue.c_str());
    if (pJsonStruct == NULL) {
        return (false);
    }

    std::map<std::string, JsonObject *>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }
        m_mapJsonObjectRef.erase(iter);
    }

    Json_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }

    return (true);
}

bool JsonObject::Replace(const std::string &strKey, int32 iValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt((uint64) iValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    std::map<std::string, JsonObject *>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }
        m_mapJsonObjectRef.erase(iter);
    }

    Json_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }
    return (true);
}

bool JsonObject::Replace(const std::string &strKey, uint32 uiValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt((uint64) uiValue, 1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    std::map<std::string, JsonObject *>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }
        m_mapJsonObjectRef.erase(iter);
    }

    Json_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }

    return (true);
}

bool JsonObject::Replace(const std::string &strKey, int64 llValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt((uint64) llValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    std::map<std::string, JsonObject *>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }
        m_mapJsonObjectRef.erase(iter);
    }

    Json_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }

    return (true);
}

bool JsonObject::Replace(const std::string &strKey, uint64 ullValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt((uint64) ullValue, 1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    std::map<std::string, JsonObject *>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }
        m_mapJsonObjectRef.erase(iter);
    }

    Json_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }

    return (true);
}

bool JsonObject::Replace(const std::string &strKey, bool bValue, bool bValueAgain) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateBool(bValue);
    if (pJsonStruct == NULL) {
        return (false);
    }

    std::map<std::string, JsonObject *>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }
        m_mapJsonObjectRef.erase(iter);
    }

    Json_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }

    return (true);
}

bool JsonObject::Replace(const std::string &strKey, float fValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateDouble((double) fValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    std::map<std::string, JsonObject *>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }
        m_mapJsonObjectRef.erase(iter);
    }

    Json_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }
    return (true);
}

bool JsonObject::Replace(const std::string &strKey, double dValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateDouble((double) dValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    std::map<std::string, JsonObject *>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }
        m_mapJsonObjectRef.erase(iter);
    }

    Json_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }

    return (true);
}

bool JsonObject::ReplaceWithNull(const std::string &strKey) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Object) {
        m_strErrMsg = "not a json object! json array?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateNull();
    if (pJsonStruct == NULL) {
        return (false);
    }

    std::map<std::string, JsonObject *>::iterator iter = m_mapJsonObjectRef.find(strKey);
    if (iter != m_mapJsonObjectRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }
        m_mapJsonObjectRef.erase(iter);
    }

    Json_ReplaceItemInObject(pFocusData, strKey.c_str(), pJsonStruct);
    if (Json_GetObjectItem(pFocusData, strKey.c_str()) == NULL) {
        return (false);
    }

    return (true);
}

int JsonObject::GetArraySize() {
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Array) {
            return (Json_GetArraySize(m_pJsonData));
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Array) {
            return (Json_GetArraySize(m_pExternJsonDataRef));
        }
    }

    return (0);
}

bool JsonObject::Get(int iWhich, JsonObject &oJsonObject) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pJsonData, iWhich);
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }

    if (pJsonStruct == NULL) {
        return (false);
    }

    char *pJsonString = Json_Print(pJsonStruct);
    std::string strJsonData = pJsonString;
    free(pJsonString);
    if (oJsonObject.Parse(strJsonData)) {
        return (true);
    } else {
        return (false);
    }
}

bool JsonObject::Get(int iWhich, std::string &strValue) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pJsonData, iWhich);
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }

    if (pJsonStruct == NULL) {
        return (false);
    }

    if (pJsonStruct->type != Json_String) {
        return (false);
    }

    strValue = pJsonStruct->valuestring;
    return (true);
}

bool JsonObject::Get(int iWhich, int32 &iValue) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pJsonData, iWhich);
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }

    if (pJsonStruct == NULL) {
        return (false);
    }

    if (pJsonStruct->type == Json_Int) {
        iValue = (int32) (pJsonStruct->valueint);
        return (true);
    } else if (pJsonStruct->type == Json_Double) {
        iValue = (int32) (pJsonStruct->valuedouble);
        return (true);
    }

    return (false);
}

bool JsonObject::Get(int iWhich, uint32 &uiValue) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pJsonData, iWhich);
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }

    if (pJsonStruct == NULL) {
        return (false);
    }

    if (pJsonStruct->type == Json_Int) {
        uiValue = (uint32) (pJsonStruct->valueint);
        return (true);
    } else if (pJsonStruct->type == Json_Double) {
        uiValue = (uint32) (pJsonStruct->valuedouble);
        return (true);
    }

    return (false);
}

bool JsonObject::Get(int iWhich, int64 &llValue) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pJsonData, iWhich);
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }

    if (pJsonStruct == NULL) {
        return (false);
    }

    if (pJsonStruct->type == Json_Int) {
        llValue = (int64) (pJsonStruct->valueint);
        return (true);
    } else if (pJsonStruct->type == Json_Double) {
        llValue = (int64) (pJsonStruct->valuedouble);
        return (true);
    }

    return (false);
}

bool JsonObject::Get(int iWhich, uint64 &ullValue) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pJsonData, iWhich);
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }

    if (pJsonStruct == NULL) {
        return (false);
    }

    if (pJsonStruct->type == Json_Int) {
        ullValue = (uint64) (pJsonStruct->valueint);
        return (true);
    } else if (pJsonStruct->type == Json_Double) {
        ullValue = (uint64) (pJsonStruct->valuedouble);
        return (true);
    }

    return (false);
}

bool JsonObject::Get(int iWhich, bool &bValue) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pJsonData, iWhich);
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }
    if (pJsonStruct == NULL) {
        return (false);
    }

    if (pJsonStruct->type > Json_True) {
        return (false);
    }

    bValue = pJsonStruct->type;
    return (true);
}

bool JsonObject::Get(int iWhich, float &fValue) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pJsonData, iWhich);
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }

    if (pJsonStruct == NULL) {
        return (false);
    }

    if (pJsonStruct->type == Json_Double) {
        fValue = (float) (pJsonStruct->valuedouble);
        return (true);
    } else if (pJsonStruct->type == Json_Int) {
        fValue = (float) (pJsonStruct->valueint);
        return (true);
    }

    return (false);
}

bool JsonObject::Get(int iWhich, double &dValue) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pJsonData, iWhich);
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }

    if (pJsonStruct == NULL) {
        return (false);
    }

    if (pJsonStruct->type == Json_Double) {
        dValue = pJsonStruct->valuedouble;
        return (true);
    } else if (pJsonStruct->type == Json_Int) {
        dValue = (double) (pJsonStruct->valueint);
        return (true);
    }

    return (false);
}

bool JsonObject::IsNull(int iWhich) const {
    Json *pJsonStruct = NULL;
    if (m_pJsonData != NULL) {
        if (m_pJsonData->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pJsonData, iWhich);
        }
    } else if (m_pExternJsonDataRef != NULL) {
        if (m_pExternJsonDataRef->type == Json_Array) {
            pJsonStruct = Json_GetArrayItem(m_pExternJsonDataRef, iWhich);
        }
    }

    if (pJsonStruct == NULL) {
        return (false);
    }

    if (pJsonStruct->type != Json_NULL) {
        return (false);
    }

    return (true);
}

bool JsonObject::Add(const JsonObject &oJsonObject) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_Parse(oJsonObject.ToString().c_str());
    if (pJsonStruct == NULL) {
        m_strErrMsg = std::string("prase json string error at ") + Json_GetErrorPtr();
        return (false);
    }

    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArray(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }

    unsigned int uiLastIndex = (unsigned int) Json_GetArraySize(pFocusData) - 1;
    for (std::map<unsigned int, JsonObject *>::iterator iter = m_mapJsonArrayRef.begin();
         iter != m_mapJsonArrayRef.end();) {
        if (iter->first >= uiLastIndex) {
            if (iter->second != NULL) {
                delete (iter->second);
                iter->second = NULL;
            }
            m_mapJsonArrayRef.erase(iter++);
        } else {
            iter++;
        }
    }

    return (true);
}

bool JsonObject::Add(const std::string &strValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateString(strValue.c_str());
    if (pJsonStruct == NULL) {
        return (false);
    }

    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArray(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }
    return (true);
}

bool JsonObject::Add(int32 iValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt((uint64) iValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArray(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }

    return (true);
}

bool JsonObject::Add(uint32 uiValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }
    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt((uint64) uiValue, 1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArray(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }

    return (true);
}

bool JsonObject::Add(int64 llValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt((uint64) llValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArray(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }

    return (true);
}

bool JsonObject::Add(uint64 ullValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt((uint64) ullValue, 1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArray(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }

    return (true);
}

bool JsonObject::Add(int iAnywhere, bool bValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateBool(bValue);
    if (pJsonStruct == NULL) {
        return (false);
    }

    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArray(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }

    return (true);
}

bool JsonObject::Add(float fValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateDouble((double) fValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArray(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }

    return (true);
}

bool JsonObject::Add(double dValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateDouble((double) dValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArray(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }

    return (true);
}

bool JsonObject::AddNull() {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateNull();
    if (pJsonStruct == NULL) {
        return (false);
    }

    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArray(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }

    return (true);
}

bool JsonObject::AddAsFirst(const JsonObject &oJsonObject) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_Parse(oJsonObject.ToString().c_str());
    if (pJsonStruct == NULL) {
        m_strErrMsg = std::string("prase json string error at ") + Json_GetErrorPtr();
        return (false);
    }

    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArrayHead(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }

    for (std::map<unsigned int, JsonObject *>::iterator iter = m_mapJsonArrayRef.begin();
         iter != m_mapJsonArrayRef.end();) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }
        m_mapJsonArrayRef.erase(iter++);
    }

    return (true);
}

bool JsonObject::AddAsFirst(const std::string &strValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateString(strValue.c_str());
    if (pJsonStruct == NULL) {
        return (false);
    }

    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArrayHead(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }

    return (true);
}

bool JsonObject::AddAsFirst(int32 iValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt((uint64) iValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArrayHead(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }

    return (true);
}

bool JsonObject::AddAsFirst(uint32 uiValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt((uint64) uiValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArrayHead(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }

    return (true);
}

bool JsonObject::AddAsFirst(int64 llValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt((uint64) llValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }
    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArrayHead(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }

    return (true);
}

bool JsonObject::AddAsFirst(uint64 ullValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt((uint64) ullValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArrayHead(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }

    return (true);
}

bool JsonObject::AddAsFirst(int iAnywhere, bool bValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateBool(bValue);
    if (pJsonStruct == NULL) {
        return (false);
    }

    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArrayHead(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }

    return (true);
}

bool JsonObject::AddAsFirst(float fValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateDouble((double) fValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArrayHead(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }

    return (true);
}

bool JsonObject::AddAsFirst(double dValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateDouble((double) dValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArrayHead(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }

    return (true);
}

bool JsonObject::AddNullAsFirst() {
    Json *pFocusData = NULL;
    if (m_pJsonData != NULL) {
        pFocusData = m_pJsonData;
    } else if (m_pExternJsonDataRef != NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        m_pJsonData = Json_CreateArray();
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateNull();
    if (pJsonStruct == NULL) {
        return (false);
    }

    int iArraySizeBeforeAdd = Json_GetArraySize(pFocusData);
    Json_AddItemToArrayHead(pFocusData, pJsonStruct);
    int iArraySizeAfterAdd = Json_GetArraySize(pFocusData);
    if (iArraySizeAfterAdd == iArraySizeBeforeAdd) {
        return (false);
    }

    return (true);
}

bool JsonObject::Delete(int iWhich) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json_DeleteItemFromArray(pFocusData, iWhich);
    for (std::map<unsigned int, JsonObject *>::iterator iter = m_mapJsonArrayRef.begin();
         iter != m_mapJsonArrayRef.end();) {
        if (iter->first >= (unsigned int) iWhich) {
            if (iter->second != NULL) {
                delete (iter->second);
                iter->second = NULL;
            }
            m_mapJsonArrayRef.erase(iter++);
        } else {
            iter++;
        }
    }

    return (true);
}

bool JsonObject::Replace(int iWhich, const JsonObject &oJsonObject) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_Parse(oJsonObject.ToString().c_str());
    if (pJsonStruct == NULL) {
        m_strErrMsg = std::string("prase json string error at ") + Json_GetErrorPtr();
        return (false);
    }

    Json_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
    if (Json_GetArrayItem(pFocusData, iWhich) == NULL) {
        return (false);
    }

    std::map<unsigned int, JsonObject *>::iterator iter = m_mapJsonArrayRef.find(iWhich);
    if (iter != m_mapJsonArrayRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }
        m_mapJsonArrayRef.erase(iter);
    }

    return (true);
}

bool JsonObject::Replace(int iWhich, const std::string &strValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateString(strValue.c_str());
    if (pJsonStruct == NULL) {
        return (false);
    }

    std::map<unsigned int, JsonObject *>::iterator iter = m_mapJsonArrayRef.find(iWhich);
    if (iter != m_mapJsonArrayRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }

        m_mapJsonArrayRef.erase(iter);
    }

    Json_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
    if (Json_GetArrayItem(pFocusData, iWhich) == NULL) {
        return (false);
    }

    return (true);
}

bool JsonObject::Replace(int iWhich, int32 iValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt((uint64) iValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    std::map<unsigned int, JsonObject *>::iterator iter = m_mapJsonArrayRef.find(iWhich);
    if (iter != m_mapJsonArrayRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }

        m_mapJsonArrayRef.erase(iter);
    }

    Json_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
    if (Json_GetArrayItem(pFocusData, iWhich) == NULL) {
        return (false);
    }

    return (true);
}

bool JsonObject::Replace(int iWhich, uint32 uiValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt((uint64) uiValue, 1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    std::map<unsigned int, JsonObject *>::iterator iter = m_mapJsonArrayRef.find(iWhich);
    if (iter != m_mapJsonArrayRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }

        m_mapJsonArrayRef.erase(iter);
    }

    Json_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
    if (Json_GetArrayItem(pFocusData, iWhich) == NULL) {
        return (false);
    }

    return (true);
}

bool JsonObject::Replace(int iWhich, int64 llValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt((uint64) ((uint64) llValue), -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    std::map<unsigned int, JsonObject *>::iterator iter = m_mapJsonArrayRef.find(iWhich);
    if (iter != m_mapJsonArrayRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }

        m_mapJsonArrayRef.erase(iter);
    }

    Json_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
    if (Json_GetArrayItem(pFocusData, iWhich) == NULL) {
        return (false);
    }

    return (true);
}

bool JsonObject::Replace(int iWhich, uint64 ullValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateInt((uint64) ullValue, 1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    std::map<unsigned int, JsonObject *>::iterator iter = m_mapJsonArrayRef.find(iWhich);
    if (iter != m_mapJsonArrayRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }

        m_mapJsonArrayRef.erase(iter);
    }

    Json_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
    if (Json_GetArrayItem(pFocusData, iWhich) == NULL) {
        return (false);
    }

    return (true);
}

bool JsonObject::Replace(int iWhich, bool bValue, bool bValueAgain) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateBool(bValue);
    if (pJsonStruct == NULL) {
        return (false);
    }

    std::map<unsigned int, JsonObject *>::iterator iter = m_mapJsonArrayRef.find(iWhich);
    if (iter != m_mapJsonArrayRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }

        m_mapJsonArrayRef.erase(iter);
    }

    Json_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
    if (Json_GetArrayItem(pFocusData, iWhich) == NULL) {
        return (false);
    }

    return (true);
}

bool JsonObject::Replace(int iWhich, float fValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateDouble((double) fValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    std::map<unsigned int, JsonObject *>::iterator iter = m_mapJsonArrayRef.find(iWhich);
    if (iter != m_mapJsonArrayRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }

        m_mapJsonArrayRef.erase(iter);
    }

    Json_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
    if (Json_GetArrayItem(pFocusData, iWhich) == NULL) {
        return (false);
    }

    return (true);
}

bool JsonObject::Replace(int iWhich, double dValue) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateDouble((double) dValue, -1);
    if (pJsonStruct == NULL) {
        return (false);
    }

    std::map<unsigned int, JsonObject *>::iterator iter = m_mapJsonArrayRef.find(iWhich);
    if (iter != m_mapJsonArrayRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }

        m_mapJsonArrayRef.erase(iter);
    }

    Json_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
    if (Json_GetArrayItem(pFocusData, iWhich) == NULL) {
        return (false);
    }

    return (true);
}

bool JsonObject::ReplaceWithNull(int iWhich) {
    Json *pFocusData = NULL;
    if (m_pJsonData == NULL) {
        pFocusData = m_pExternJsonDataRef;
    } else {
        pFocusData = m_pJsonData;
    }

    if (pFocusData == NULL) {
        m_strErrMsg = "json data is null!";
        return (false);
    }

    if (pFocusData->type != Json_Array) {
        m_strErrMsg = "not a json array! json object?";
        return (false);
    }

    Json *pJsonStruct = Json_CreateNull();
    if (pJsonStruct == NULL) {
        return (false);
    }

    std::map<unsigned int, JsonObject *>::iterator iter = m_mapJsonArrayRef.find(iWhich);
    if (iter != m_mapJsonArrayRef.end()) {
        if (iter->second != NULL) {
            delete (iter->second);
            iter->second = NULL;
        }

        m_mapJsonArrayRef.erase(iter);
    }

    Json_ReplaceItemInArray(pFocusData, iWhich, pJsonStruct);
    if (Json_GetArrayItem(pFocusData, iWhich) == NULL) {
        return (false);
    }

    return (true);
}

JsonObject::JsonObject(Json *pJsonData)
        : m_pJsonData(NULL), m_pExternJsonDataRef(pJsonData) {
}
