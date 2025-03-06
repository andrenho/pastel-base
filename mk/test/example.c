#include <stdio.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "hello.txt.h"

int main()
{
    printf("Imported: %s\n", (const char *) hello_txt);

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dostring(L, "print('Hello from Lua!')");
    lua_close(L);
}