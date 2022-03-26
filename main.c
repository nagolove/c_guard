#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "mem_guard.h"

struct Byte {
    unsigned char _0: 1;
    unsigned char _1: 1;
    unsigned char _2: 1;
    unsigned char _3: 1;
    unsigned char _4: 1;
    unsigned char _5: 1;
    unsigned char _6: 1;
    unsigned char _7: 1;
};

// Буфер для записы должен быть как минимум sizeof(value) * 8 + 9
void uint64t_to_bitstr(uint64_t value, char *buf) {
    assert(buf && "buf should not be a nil");
    char *ptr = (char*)&value;
    char *last = buf;

    union BitMap {
        struct Byte b[8];
        uint64_t u;
    } bp = { .u = value, };

    for(int i = 0; i < sizeof(value); ++i) {
        last += sprintf(last, "%d", (int)bp.b[i]._0);
        last += sprintf(last, "%d", (int)bp.b[i]._1);
        last += sprintf(last, "%d", (int)bp.b[i]._2);
        last += sprintf(last, "%d", (int)bp.b[i]._3);
        last += sprintf(last, "%d", (int)bp.b[i]._4);
        last += sprintf(last, "%d", (int)bp.b[i]._5);
        last += sprintf(last, "%d", (int)bp.b[i]._6);
        last += sprintf(last, "%d", (int)bp.b[i]._7);
        last += sprintf(last, " ");
    }
}

void test_uint64t_to_bitstr(uint64_t value) {
    char buf[256] = {0, };
    uint64t_to_bitstr(value, buf);
    printf("%u = %s\n", value, buf);
    printf("-------------------------------------------------\n");
}

int main(void) {
    /*malloc(12);*/

    lua_State *lua = luaL_newstate();

    size_t sz = 101;
    void *ud = new_guarded_ud(lua, sz);

    strcpy((char*)ud, "strokovui_literal_mmm");
    char *shifted_ptr = ((char*)ud) - 1;
    /*memset(shifted_ptr, 'z', sz);*/

    /*luaL_ref(lua, LUA_REGISTRYINDEX);*/
    /*lua_objlen(lua*/

    printf("check %d\n", (int)check_guarded_ud(ud, 0));

    lua_close(lua);

    printf("sizeof(uintptr_t) = %u\n", sizeof(uintptr_t));

    test_uint64t_to_bitstr(0);
    test_uint64t_to_bitstr(1);
    test_uint64t_to_bitstr(2);
    test_uint64t_to_bitstr(10);
    test_uint64t_to_bitstr(8);
    test_uint64t_to_bitstr(132413);
    test_uint64t_to_bitstr(65535);
    test_uint64t_to_bitstr(65536);

    return EXIT_SUCCESS;
}
