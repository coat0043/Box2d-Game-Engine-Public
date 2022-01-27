#pragma once

// This file includes many static headers and must be included by all files
//   inside this Framework project.
// Any changes to any of these headers will cause a full rebuild of the
//   Framework project, which isn't desirable.

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

#include <algorithm>
#include <random>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>
#include <fstream>  
#include <iostream>
#include <filesystem>

#include <map>
#include <vector>
#include <queue>
#include <string>

#include <Xinput.h>
#include <xaudio2.h>

#include <GL/GL.h>
#include "GL/glext.h"
#include "GL/wglext.h"

#if FW_USE_LUA
#define FW_USING_LUA 1
#else
#define FW_USING_LUA 0
#endif

// Lua.
#if FW_USING_LUA
extern "C"
{
#include "../Libraries/Lua/src/lua.h"
#include "../Libraries/Lua/src/lualib.h"
#include "../Libraries/Lua/src/lauxlib.h"
}
#pragma warning( push )
#pragma warning( disable : 4640 )
#include "LuaBridge/LuaBridge.h"
#pragma warning( pop )
#endif //FW_USING_LUA

#include "GL/GLExtensions.h"
#include "GL/WGLExtensions.h"

#include "../Libraries/imgui/imgui.h"
#include "../Libraries/ImFileDialog/ImFileDialog.h"
#include "../Libraries/rapidjson/include/rapidjson/document.h"
#include "../Libraries/rapidjson/include/rapidjson/filewritestream.h"
#include "../Libraries/rapidjson/include/rapidjson/writer.h"
#include "../Libraries/rapidjson/include/rapidjson/prettywriter.h"

#include "Memory/MyMemory.h"
#include "UI/Log.h"
