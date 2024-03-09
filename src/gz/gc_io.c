#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include "gc_io.h"

#define GC_RDB_REG (*(volatile uint32_t *)0xA4900000)

static void gc_output_string(const char *str)
{
  while (1) {
    if (str[0] == '\0')
    {
      GC_RDB_REG = (1 << 26) | (0 << 24);
      break;
    }
    else if (str[1] == '\0')
    {
      GC_RDB_REG = (1 << 26) | (1 << 24) | (str[0] << 16);
      break;
    }
    else if (str[2] == '\0')
    {
      GC_RDB_REG = (1 << 26) | (2 << 24) | (str[0] << 16) | (str[1] << 8);
      break;
    }
    else
    {
      GC_RDB_REG = (1 << 26) | (3 << 24) | (str[0] << 16) | (str[1] << 8) | (str[2] << 0);
      str += 3;
    }
  }
}

void gc_printf(const char *fmt, ...)
{
  char buf[256];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);
  gc_output_string(buf);
}
