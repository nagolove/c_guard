#include <lua.h>
#include <stdio.h>
#include <stdbool.h>

extern void *new_guarded_ud(lua_State *lua, size_t size);
extern bool check_guarded_ud(void *ud, size_t sz);
