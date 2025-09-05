#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

static void _reverse(char *s, int len){
  int i = 0, j = len - 1;
  while (i < j){
    char temp = s[i];
    s[i] = s[j];
    s[j] = temp;
    i++;
    j--;
  }
} 

static int _itoa(int n, char *s) {
  bool is_neg = false;
  int i = 0;

  if (n == 0) {
    *s++ = '0';
    *s = '\0';
    return 1;
  } else if (n < 0) {
    n = -n;
    is_neg = true;
  }

  while (n != 0){
    int digit = n % 10;
    s[i++] = '0' + digit;
    n /= 10;
  }

  if (is_neg) {
    s[i++] = '-';
  }

  _reverse(s, i);
  s[i] = '\0';
  return i;

}

int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  
  int count = 0;

  char buffer[32];
   
  while (*fmt != '\0'){
    if (*fmt != '%'){
      *out = *fmt;
      out++;
      count++;
    }
    else {
      fmt++;
      switch (*fmt) {
        case 'd':{
          int num = va_arg(args, int);
          int len = _itoa(num, buffer);
          for (int i = 0; i < len; i++){
            *out = buffer[i];
            out++;
            count++;
          }
          break;
        }
        case 's':{
          const char *str = va_arg(args, const char *);
          while (*str != '\0'){
            *out = *str;
            out++;
            str++;
            count++;
          }
          break;
        }
        default:{
          *out = '%';
          out++;
          *out = *fmt;
          out++;
          count+=2;
          break;
        }
      }
    }

    fmt++;
  }
  *out = '\0';
  va_end(args);

  return count;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
