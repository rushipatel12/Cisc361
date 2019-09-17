#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 128

typedef struct mp3 {
  char *name;
  struct mp3 *next;
} mp3_t;

int main (int argc, char *argv[])
{
  char  buffer[BUFFERSIZE];
  mp3_t *mp3;
  int   len;

  printf("Enter a name: ");
  if (fgets(buffer, BUFFERSIZE , stdin) != NULL)
  {
    len = (int) strlen(buffer);
    printf("length [%d] of string %s", len, buffer); // notice the length!
    buffer[len - 1] = '\0';                 // why minus 1 ???
    mp3 = (mp3_t *) malloc(sizeof(mp3_t));
    mp3->name = (char *) malloc(len);
    strcpy(mp3->name, buffer);
    printf("Name is [%s]...\n", mp3->name);
  }
  free(mp3->name);   // line free 1
  free(mp3);         // line free 2
  return 0;
}