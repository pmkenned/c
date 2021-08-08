#include <stdio.h>
#include <stdint.h>

typedef uint8_t  u8;
typedef int8_t   s8;
typedef uint16_t u16;
typedef int16_t  s16;
typedef uint32_t u32;
typedef int32_t  s32;
typedef uint64_t u64;
typedef int64_t  s64;

int main()
{
    u32 x = 5;
    printf(__FILE__ ":%d: %d\n", __LINE__, x); return 0;
}
