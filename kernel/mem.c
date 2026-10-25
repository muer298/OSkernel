#include <stddef.h>

/* Freestanding compiler-generated memory operations may require these. */
void *memset(void *dst, int value, size_t length)
{
    unsigned char *p = (unsigned char *)dst;
    for (size_t i = 0; i < length; i++) {
        p[i] = (unsigned char)value;
    }
    return dst;
}

void *memcpy(void *dst, const void *src, size_t length)
{
    unsigned char *d = (unsigned char *)dst;
    const unsigned char *s = (const unsigned char *)src;
    for (size_t i = 0; i < length; i++) {
        d[i] = s[i];
    }
    return dst;
}
