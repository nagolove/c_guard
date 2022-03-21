#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "mem_guard.h"

int main(void) {
    /*malloc(12);*/

    lua_State *lua = luaL_newstate();

    size_t sz = 101;
    void *ud = new_guarded_ud(lua, sz);
    /*strcpy((char*)ud, "strokovui_literal_mmm");*/
    memset(ud, 'z', sz);

    printf("check %d\n", (int)check_guarded_ud(ud, 0));

    lua_close(lua);

    return EXIT_SUCCESS;
}
