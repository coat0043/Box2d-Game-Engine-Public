#pragma once

#include "Math/Vector.h"

namespace fw {

typedef rapidjson::PrettyWriter<rapidjson::StringBuffer> WriterType;

void JSONSaveCharArray(WriterType& writer, const char* key, const char* type);
void JSONSaveBool(WriterType& writer, const char* key, bool value);
void JSONSaveFloat(WriterType& writer, const char* key, float value);
void JSONSaveInt(WriterType& writer, const char* key, int value);
void JSONSaveVec2(WriterType& writer, const char* key, vec2 value);
void JSONSaveVec3(WriterType& writer, const char* key, vec3 value);

void JSONLoadInt(rapidjson::Value& object, const char* key, int* value);
void JSONLoadBool(rapidjson::Value& object, const char* key, bool* value);
void JSONLoadFloat(rapidjson::Value& object, const char* key, float* value);
void JSONLoadVec2(rapidjson::Value& object, const char* key, vec2* value);
void JSONLoadVec3(rapidjson::Value& object, const char* key, vec3* value);

} // namespace fw
