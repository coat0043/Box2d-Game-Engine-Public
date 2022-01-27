#pragma once

#if FW_USING_LUA
namespace fw
{

class LuaScript
{
public:
    LuaScript(const char* fileName);
    ~LuaScript();

private:
};

}
#endif