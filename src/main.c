#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <stdbool.h>

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

const char *guard_def = "abcdefg";

void *new_guarded_user_data(lua_State *lua, size_t size) {
    size_t full_size = 
        size + 
        strlen(guard_def) * 2 + // длина хранителей
        2 +                     // два терминальных нуля
        sizeof(size_t);         // для хранения размера выделенного куска
    void *ud = lua_newuserdata(lua, full_size);
    memset(ud, 0, full_size);

    //char *ret = strcpy((char*)ud, guard_def);
    //*++ret = 0;
    //*(size_t*)ret = size;
    //ret += sizeof(size);
    //char *last = (char*)ud + strlen(guard_def) + size + 1;
    //ret = strcpy(last, guard_def);
    //*++ret = 0;

    char *ret = strcpy((char*)ud, guard_def);
    *++ret = 0;
    ret += sizeof(size_t);
    *(size_t*)ret = size;

    ret += size;
    char *last = ret;
    /*char *last = (char*)ud + strlen(guard_def) + size + 1;*/

    ret = strcpy(last, guard_def);
    *++ret = 0;

    return (char*)ud + strlen(guard_def) + 1 + sizeof(size_t);
}

bool check_guarded_user_data(void *ud) {
    char *first = (char*)ud - strlen(guard_def) - 1 - sizeof(size_t);
    /*char *second = (char*)ud;*/
    printf("first %s\n", first);

    return false;
}

int main(void) {
    malloc(12);
    lua_State *lua = luaL_newstate();

    void *ud = new_guarded_user_data(lua, 100);
    strcpy((char*)ud, "strokovui_literal_mmm");
    check_guarded_user_data(ud);

    lua_close(lua);

    return EXIT_SUCCESS;
}
