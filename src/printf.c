#include "sbi.h"
#include "printf.h"
#include <stdarg.h>

void putchar(char c){
    PUTCHAR(c);
}


static char digits[] = "0123456789abcdef";

// base进制 sign有符号或无符号
static void
printint(int xx, int base, int sign)
{
  char buf[16];
  int i;
  unsigned x;

  if(sign && (sign = xx < 0))
    x = -xx;
  else
    x = xx;

  i = 0;
  do {
    buf[i++] = digits[x % base];
  } while((x /= base) != 0);

  if(sign)
    buf[i++] = '-';

  while(--i >= 0)
    putchar(buf[i]);
}

static void
printptr(unsigned long x)
{
  int i;
  putchar('0');
  putchar('x');
  for (i = 0; i < (sizeof(unsigned long) * 2); i++, x <<= 4)
    putchar(digits[x >> (sizeof(unsigned long) * 8 - 4)]);
}


// Print to the console. only understands %d, %x, %p, %s.
void
printf(char *fmt, ...)
{
  va_list ap;
  int i, c;
  char *s;

  if (fmt == 0)
    panic("null fmt");

  va_start(ap, fmt);
  for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
    if(c != '%'){
      putchar(c);
      continue;
    }
    c = fmt[++i] & 0xff;
    if(c == 0)
      break;
    switch(c){
    case 'd':
      printint(va_arg(ap, int), 10, 1);
      break;
    case 'x':
      printint(va_arg(ap, int), 16, 1);
      break;
    case 'p':
      printptr(va_arg(ap, unsigned long));
      break;
    case 's':
      if((s = va_arg(ap, char*)) == 0)
        s = "(null)";
      for(; *s; s++)
        putchar(*s);
      break;
    case '%':
      putchar('%');
      break;
    default:
      // Print unknown % sequence to draw attention.
      putchar('%');
      putchar(c);
      break;
    }
  }
  va_end(ap);
}

void
panic(char *s)
{
  printf("panic: ");
  printf(s);
  printf("\n");
  for(;;)
    ;
}