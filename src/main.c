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

typedef struct {
    struct GuardedBlock *next;
    void *ptr;
    size_t size;
} GuardedBlock;

GuardedBlock *guarded_blocks_root = NULL;

void *new_guarded_user_data(lua_State *lua, size_t size) {
    size_t full_size = 
        size + 
        strlen(guard_def) * 2 + // длина строчек хранителей
        2 +                     // два терминальных нуля
        sizeof(size_t);         // для хранения размера выделенного куска
    char *ptr = lua_newuserdata(lua, full_size);
    memset(ptr, 0, full_size);

    ptr += sprintf(ptr, guard_def) + 1;
    *(size_t*)ptr = size;
    ptr += sizeof(size_t);
    void *ret = ptr;
    ptr += size;
    sprintf(ptr, guard_def);

    return ret;
}

bool check_guarded_user_data(void *ud) {
    char *ptr = ud;
    char *first = ptr - strlen(guard_def) - 1 - sizeof(size_t);

    // Превентивная установка конечного нуля на строчку если он был затерт.
    char *null_zero = ptr - sizeof(size_t) - 1;
    *null_zero = 0;

    size_t size = *(size_t*)(ptr - strlen(guard_def) - 1);
    char *second = ptr + size;
    
    /*printf("fir '%s'\n", first);*/
    /*printf("sec '%s'\n", second);*/
    /*printf("size %d\n", (int)size);*/

    return !strcmp(first, second);
}

int main(void) {
    malloc(12);
    lua_State *lua = luaL_newstate();

    size_t sz = 101;
    void *ud = new_guarded_user_data(lua, sz);
    /*strcpy((char*)ud, "strokovui_literal_mmm");*/
    memset(ud, 'z', sz);

    printf("check %d\n", (int)check_guarded_user_data(ud));

    lua_close(lua);

    return EXIT_SUCCESS;
}
