#include "FrameworkPCH.h"
#include "JSONHelpers.h"
#include "Math/Vector.h"

namespace fw {

void JSONSaveCharArray(WriterType& writer, const char* key, const char* type)
{
    writer.Key(key);
    writer.String(type);
}

void JSONSaveBool(WriterType& writer, const char* key, bool value)
{
    writer.Key(key);
    writer.Bool(value);
}

void JSONSaveInt(WriterType& writer, const char* key, int value)
{
    writer.Key(key);
    writer.Int(value);
}

void JSONSaveFloat(WriterType& writer, const char* key, float value)
{
    writer.Key(key);
    writer.Double(value);
}

void JSONSaveVec2(WriterType& writer, const char* key, vec2 value)
{
    writer.Key(key);
    writer.StartArray();
    writer.Double(value.x);
    writer.Double(value.y);
    writer.EndArray();
}

void JSONSaveVec3(WriterType& writer, const char* key, vec3 value)
{
    writer.Key(key);
    writer.StartArray();
    writer.Double(value.x);
    writer.Double(value.y);
    writer.Double(value.z);
    writer.EndArray();
}

void JSONLoadInt(rapidjson::Value& object, const char* key, int* value)
{
    assert( value != nullptr );

    if( object.HasMember( key ) )
    {
        *value = object[key].GetInt();
    }
}

void JSONLoadBool(rapidjson::Value& object, const char* key, bool* value)
{
    assert( value != nullptr );

    if( object.HasMember( key ) )
    {
        *value = object[key].GetBool();
    }
}

void JSONLoadFloat(rapidjson::Value& object, const char* key, float* value)
{
    assert( value != nullptr );

    if( object.HasMember( key ) )
    {
        *value = object[key].GetFloat();
    }
}

void JSONLoadVec2(rapidjson::Value& object, const char* key, vec2* value)
{
    assert( value != nullptr );

    if( object.HasMember( key ) )
    {
        value->x = object[key][0].GetFloat();
        value->y = object[key][1].GetFloat();
    }
}

void JSONLoadVec3(rapidjson::Value& object, const char* key, vec3* value)
{
    assert( value != nullptr );

    if( object.HasMember( key ) )
    {
        value->x = object[key][0].GetFloat();
        value->y = object[key][1].GetFloat();
        value->z = object[key][2].GetFloat();
    }
}

} // namespace fw
