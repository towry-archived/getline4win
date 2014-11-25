/* 
 contributed by Towry Wang, the original author 
 is ivanra on github.
 2014-11-25 16:37
*/
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <malloc.h>

/* initialize variable before use it */

#if defined(_WIN64) || defined(_WIN32)
#if defined(_WIN64)
 typedef long int ssize_t;
#else
 typedef int ssize_t;
#endif

ssize_t
getdelim(char **lineptr, size_t *n, int delim, FILE *stream)
{
  char c, *cp, *new_lineptr;

  if (lineptr == NULL || n == NULL || stream == NULL) {
    errno = EINVAL;
    return -1;
  }

  if (*lineptr == NULL) {
    *n = 128;
    if ((*lineptr = (char *)malloc(*n)) == NULL) {
      errno = ENOMEM;
      return -1;
    }
  }

  cp = *lineptr;
  for (;;) {

    c = getc(stream);
    if (ferror(stream) || (c == EOF && cp == *lineptr))
      return -1;

    if (c == EOF)
      break;

    if ((*lineptr + *n - cp) < 2) {
#ifdef _POSIX_
      if (SSIZE_MAX / 2 < *n) {
#else
      if (SIZE_MAX / 2 < *n) {
#endif
        #ifdef EOVERFLOW
          errno = EOVERFLOW;
        #else
          errno = ERANGE;
        #endif
        return -1;
      }
      *n = *n * 2;
      if ((new_lineptr = (char*)realloc(*lineptr, *n)) == NULL) {
        errno = ENOMEM;
        return -1;
      }
      cp = new_lineptr + (cp - *lineptr);
      *lineptr = new_lineptr;
    }
    *cp++ = c;

    if (c == delim)
      break;
  }
  *cp = '\0';
  return (size_t)(cp - *lineptr);
}

ssize_t
getline(char **lineptr, size_t *n, FILE *stream)
{
  return getdelim(lineptr, n, '\n', stream);
}
#endif
