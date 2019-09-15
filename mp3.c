#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mp3.h"

#define BUFFERSIZE 128

typedef struct mp3 {
    char *artist;
    char *songName;
    int year;
    int runtime;
    struct mp3 *next;
    struct mp3 *prev;
} mp3_t;

void enter(mp3_t **first ){
    char  buffer[BUFFERSIZE];
    int   len;
    mp3_t *mp3;
    mp3_t *tmp;
    tmp = *first;
    while(tmp->next != NULL){
      tmp = tmp->next;
    }
    static int nodeCount = 0;
    printf("Enter a name: ");
  if (fgets(buffer, BUFFERSIZE , stdin) != NULL)
  {
    len = (int) strlen(buffer);
    buffer[len - 1] = '\0';                 
    mp3 = (mp3_t *) malloc(sizeof(mp3_t));
    mp3->artist = (char *) malloc(len);
    strcpy(mp3->artist, buffer);
  }


  printf("Enter a song: ");
  if (fgets(buffer, BUFFERSIZE , stdin) != NULL)
  {
    len = (int) strlen(buffer);
    buffer[len - 1] = '\0';                 
    mp3->songName = (char *) malloc(len);
    strcpy(mp3->songName, buffer);
  }
    printf("Enter the Year of the Song: ");
    scanf("%d",&mp3->year);
    printf("Enter the Duration of the Song: ");
    scanf("%d",&mp3->runtime);
    getchar();

    printf("Name is [ %s]...\n", mp3->artist);
    printf("Name is [%s]...\n", mp3->songName);
    printf("Year: %d\n",mp3->year);
    printf("Duration: %d\n",mp3->runtime);

    nodeCount++;
    if(nodeCount == 1 || first == NULL){
        **first = *mp3;
        mp3->next = NULL;
        mp3->prev = NULL;
    }
    else{
        tmp->next = mp3;
        mp3->prev = tmp;
        mp3->next = NULL;
    }


}

void delete(mp3_t **first){
  printf("CAllED1");
  char  buffer[BUFFERSIZE];
  int   len;
  char *artistDel;
  mp3_t *tmp;
  tmp = *first;
  mp3_t *tmpFirst;
  tmpFirst = *first;
  printf("Enter the Name of the Artist you would like to delete: ");
  if (fgets(buffer, BUFFERSIZE , stdin) != NULL)
  {
    len = (int) strlen(buffer);
    printf("length [%d] of string %s", len, buffer); 
    buffer[len - 1] = '\0';                 
    artistDel = (char *) malloc(len);
    strcpy(artistDel, buffer);
    printf("Name is [%s]...\n", artistDel);
  }

  while(tmp != NULL){
    printf("enterd");
    if(strcmp(tmp->artist,artistDel)==0){
        deleteMP3(tmp,first);
    }
    tmp = tmp->next;
  }


}

void deleteMP3(mp3_t *deleteArtist, mp3_t **tmpFirst){
  printf("CAllED2");
  if(*tmpFirst == NULL){
    return;
  }
  if(deleteArtist->prev == NULL){
    printf("first");
    *tmpFirst = deleteArtist->next;
  }
  if(deleteArtist->next != NULL){
    deleteArtist->next->prev = deleteArtist->prev;
  }
  if(deleteArtist->prev != NULL){
    deleteArtist->prev->next = deleteArtist->next;
  }

}


void printListForward(struct mp3* first){
  if (first == NULL) { 
      printf("theres nothing in this list");
  }
  else{
    printf("These are all the Mp3's in the Linked List Forward\n");
    while(first != NULL){
      printf("Name is [%s], ", first->artist);
      printf("Song title is [%s], ", first->songName);
      printf("Year is: [%d], ", first->year);
      printf("Runtime is [%d}.", first->runtime);
      printf("\n");
      first=first->next;
    }
  }
}

void printListReverse(mp3_t *first){
  if(first == NULL){ 
    printf("theres nothing in this list");
  }
else{
  mp3_t *last = first;
  while(last->next != NULL){
    last = last->next;
  }
  printf("These are all the Mp3's in the Linked List in Reverse\n");
  while(last != NULL){
    printf("");
    printf("Name is [%s], ", last->artist);
    printf("Song title is [%s], ", last->songName);
    printf("Year is: [%d], ", last->year);
    printf("Runtime is [%d}.", last->runtime);
    printf("\n");
    last=last->prev;
  }
  }
}



void freeList(mp3_t *first)
{
  mp3_t *tmp;
  tmp = first;
  while(first != NULL){
    tmp = first;
    first = first->next;
    free(tmp);
  }

}

int main(){
    mp3_t *first;
    enter(&first);
    enter(&first);
    // enter(&first);
    delete(&first);
    printListForward(first);
    freeList(first);
	return 0;
}