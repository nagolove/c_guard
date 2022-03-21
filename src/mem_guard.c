#include <lua.h>
#include <stdio.h>
#include <stdbool.h>
#include <memory.h>
#include <string.h>

/*
typedef struct {
    struct GuardedBlock *next;
    void *ptr;
    size_t size;
} GuardedBlock;

GuardedBlock *guarded_blocks_root = NULL;
*/

static const char *guard_def = "abcdefg";

void *new_guarded_ud(lua_State *lua, size_t size) {
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

bool check_guarded_ud(void *ud, size_t sz) {
    char *ptr = ud;
    char *first = ptr - strlen(guard_def) - 1 - sizeof(size_t);

    // Превентивная установка конечного нуля на строчку если он был затерт.
    char *null_zero = ptr - sizeof(size_t) - 1;
    *null_zero = 0;

    size_t preffered_size = 0;
    if (sz == 0)
        preffered_size = *(size_t*)(ptr - strlen(guard_def) - 1);
    else
        preffered_size = sz;

    char *second = ptr + preffered_size;
    
    /*printf("fir '%s'\n", first);*/
    /*printf("sec '%s'\n", second);*/
    /*printf("size %d\n", (int)size);*/

    return !strncmp(first, second, strlen(guard_def));
}
