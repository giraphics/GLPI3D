#ifndef LUA_SAMPLE1_H
#define LUA_SAMPLE1_H
#include <lua.hpp>                                /* Always include this when calling Lua */
//#include <lauxlib.h>                            /* Always include this when calling Lua */
//#include <lualib.h>                             /* Always include this when calling Lua */
#include <iostream>
void bail(lua_State *L, char *msg){
	fprintf(stderr, "\nFATAL ERROR:\n  %s: %s\n\n",
		msg, lua_tostring(L, -1));
	exit(1);
}

char *szLua =
"x = 8 "
"return ( x > 7 ) ";

int lua()
{
    lua_State *lState;

    lState = luaL_newstate();
    luaL_openlibs( lState );

    int iStatus = luaL_loadstring( lState, szLua );
    if( iStatus )
    {
        std::cout << "Error: " << lua_tostring( lState, -1 );
        return 1;
    }
    iStatus = lua_pcall( lState, 0, LUA_MULTRET, 0 );
    if( iStatus )
    {
        std::cout << "Error: " << lua_tostring( lState, -1 );
        return 1;
    }

    int iRet = (int) lua_toboolean( lState, -1 );
    if( iRet )
    {
        std::cout << "Door opened!" << std::endl;
    }
    else
    {
        std::cout << "Door still closed." << std::endl;
    }

    lua_close( lState );
	return 0;
}

void Lua_Sample1(){
   lua();
}


#endif
