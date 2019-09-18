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

void enterData(mp3_t **first, mp3_t **tail ){
    char  buffer[BUFFERSIZE];
    int   len;
    mp3_t *mp3 = NULL;
    printf("Enter an Artist: ");
    getchar();
  if (fgets(buffer, BUFFERSIZE , stdin) != NULL)
  {
    len = (int) strlen(buffer);
    buffer[len - 1] = '\0';                 
    mp3 = (mp3_t *) malloc(sizeof(mp3_t));
    mp3->next = mp3->prev = NULL;
    mp3->artist = (char *) malloc(len);
    strcpy(mp3->artist, buffer);
  }


  printf("Enter a Song Title: ");
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

    if(*first == NULL){
        *first = mp3;
        *tail = mp3;
        (*first)->prev = NULL;
    }
    else{
        mp3->prev = *tail;
        (*tail)->next = mp3;
        *tail = mp3;
    }


}

void removeNode(mp3_t **first, mp3_t **tail){
  char  buffer[BUFFERSIZE];
  int   len;
  char *artistDel;
  mp3_t *tmp = NULL;
  tmp = *first;
  mp3_t *tmpDel = NULL;
  getchar();
  printf("Enter the Name of the Artist you would like to delete: ");
  if (fgets(buffer, BUFFERSIZE , stdin) != NULL)
  {
    len = (int) strlen(buffer);
    buffer[len - 1] = '\0';                 
    artistDel = (char *) malloc(len);
    strcpy(artistDel, buffer);
  }

  while(tmp != NULL){
    if(strcmp(tmp->artist,artistDel)==0){
        tmpDel = tmp;
        tmp = tmp->next;
        removeMP3(tmpDel,first,tail);
    }
    else{
      tmp = tmp->next;
    }

  }
  free(artistDel);



}

void removeMP3(mp3_t *deleteArtist, mp3_t **tmpFirst, mp3_t **tmpTail){
  mp3_t *tmpFree = NULL;
  tmpFree = deleteArtist;
  if(!(*tmpFirst)){
    return;
  }
  if(deleteArtist->prev == NULL){
    *tmpFirst = deleteArtist->next;
  }
  if(deleteArtist->next == NULL){
    *tmpTail = deleteArtist->prev;
  }
  if(deleteArtist->next != NULL){
    deleteArtist->next->prev = deleteArtist->prev;
  }
  if(deleteArtist->prev != NULL){
    deleteArtist->prev->next = deleteArtist->next;
  }
  free(tmpFree->artist);
  free(tmpFree->songName);
  free(tmpFree);


}


void forwardPrinting(mp3_t *first){
  if (first == NULL) { 
      printf("There is no MP3 data in the List");
  }
  else{
    printf("This is MP3 data in the order it was added (Forward):\n");
    while(first != NULL){
      printf("The Artist is %s, ", first->artist);
      printf("The Song title is %s, ", first->songName);
      printf("The Year is: %d, ", first->year);
      printf("and the Duration is %d minutes. \n", first->runtime);
      first=first->next;
    }
  }
}

void reversePrinting(mp3_t *tail){
  if(tail == NULL){ 
    printf("There is no MP3 data in the List");
  }
  else{
    printf("This is the MP3 data by the most recent addition (Reverse)\n");
    while(tail != NULL){
      printf("The Artist is %s, ", tail->artist);
      printf("The Song title is %s, ", tail->songName);
      printf("The Year is: %d, ", tail->year);
      printf("and the Duration is %d minutes. \n", tail->runtime);
      tail = tail->prev;
    }
}
}



void clearHeap(mp3_t *first){
  mp3_t *tmp = NULL;
  tmp = first;
  while(first != NULL){
    tmp = first;
    first = first->next;
    free(tmp->artist);
    free(tmp->songName);
    free(tmp);
  }
  

}

int main(){
    mp3_t *first = NULL;
    mp3_t *tail = NULL;

    char choices;
    printf("MP3 Doubly Linked List");
    printf("\n\n");
    
    do{
      printf("Choices\n");
      printf("1. Add MP3 data to the List\n");
      printf("2. Delete an Artist from the List\n");
      printf("3. Print the MP3 data in the order it was added (Forward)\n");
      printf("4. Print the MP3 data by the most recent addition (Reverse)\n");
      printf("5. Exit the program\n");
      printf("Pick a choice from above: ");
      scanf(" %c",&choices);

      switch(choices){
        case '1':
          enterData(&first,&tail);
          break;
        case '2':
          removeNode(&first,&tail);
          break;
        case '3':
          forwardPrinting(first);
          break;
        case '4':
          reversePrinting(tail);
          break;
        case '5':
          break;
        default:
          printf("Not a choice");
          break;
      } 
    } while(choices != 'E');
      clearHeap(first);
	return 0;
}