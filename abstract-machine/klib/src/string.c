#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  if (s == NULL) {
    return 0;
  }
  
  size_t len = 0;
  while(s[len] != '\0') {
    len++;
  }
  return len;
  
}

char *strcpy(char *dst, const char *src) {
  if (dst == NULL || src == NULL) {
    return NULL;
  }

  char *ret = dst;
  while (*src != '\0'){
    *dst = *src;
    dst++;
    src++;
  }
  *dst = '\0';
  return ret;
}

char *strncpy(char *dst, const char *src, size_t n) {
  if (dst == NULL || src == NULL) {
    return NULL;
  }

  char *ret = dst;
  size_t i;
  for (i = 0; i < n && src[i] != '\0'; i++) {
    dst[i] = src[i];
  }
    
  for (; i < n; i++) {
    dst[i] = '\0';
  }
  return ret;
}

char *strcat(char *dst, const char *src) {
  size_t len_dst= strlen(dst);
  strcpy(dst + len_dst, src);
  return dst;

}

int strcmp(const char *s1, const char *s2) {
  size_t i = 0;
  while (s1[i] != '\0' && s1[i] != s2[i]) {
    i++;
  }
  return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int strncmp(const char *s1, const char *s2, size_t n) {
  size_t i = 0;
  while (s1[i] != '\0' && s1[i] != s2[i] && i < n) {
    i++;
  }
  if (i == n) return 0;
  return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void *memset(void *s, int c, size_t n) {
  for (size_t i = 0;i < n; i++) {
    ((unsigned char *)s)[i] = (unsigned char)c;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  if (dst == src || n == 0) {
    return dst;
  }
  else if (dst < src){
    for (size_t i = 0; i < n; i++) {
      ((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
    }
  }
  else{
    for (size_t i = n; i > 0; i--) {
      ((unsigned char *)dst)[i - 1] = ((unsigned char *)src)[i - 1];
    }
  }

  return dst;
}

void *memcpy(void *out, const void *in, size_t n) {
  for (size_t i = 0; i < n; i++) {
    ((unsigned char *)out)[i] = ((unsigned char *)in)[i];
  }
  return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  size_t i = 0;
  while (i < n) {
    if (((unsigned char *)s1)[i] != ((unsigned char *)s2)[i]) {
      return ((unsigned char *)s1)[i] - ((unsigned char *)s2)[i];
    }
    i++;
  }
  return 0;
}

#endif
