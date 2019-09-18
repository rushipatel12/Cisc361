/**
 * In this file you find the implemented methods from the mp3.h file.
 * First we declare a struct of type mp3 and alias it as mp3_t, this struct has
 * two char attributes, two int attributes, and two mp3 attributes.
 * Below the mp3 declaration we have all of the method implementations followed by the
 * main where the program is brought together in a menu.
 **/

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

/**
 * The enterData method takes in two parameters of type mp3 which holds the address 
 * of a pointer. This method then asks the user for information to create an MP3 node. 
 * It then adds the new node that was created to the end of the doubly linked list.
 * There are three side effects from calling this method because it creates space on the heap
 * for the new mp3 node, and the two character arrays for artist and song title.
 **/

void enterData(mp3_t **first, mp3_t **tail ){
  char buffer[BUFFERSIZE];
  int len;
  mp3_t *mp3 = NULL;
  printf("Enter an Artist: ");
  getchar();
  if (fgets(buffer, BUFFERSIZE , stdin) != NULL){
    len = (int) strlen(buffer);
    buffer[len - 1] = '\0';                 
    mp3 = (mp3_t *) malloc(sizeof(mp3_t));
    mp3->next = mp3->prev = NULL;
    mp3->artist = (char *) malloc(len);
    strcpy(mp3->artist, buffer);
  }

  printf("Enter a Song Title: ");
  if (fgets(buffer, BUFFERSIZE , stdin) != NULL){
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

/**
 * The removeNode method takes in two parameters of type mp3 which holds the address 
 * of a pointer. This method will then ask the user for the artist they would like to remove from the list.
 * It then traverses the entire list and checks for the artist name and if they match it will call the 
 * removeMP3 method. Then it will free the space that was allocated for the artist to be deleted 
 * character array. There are no side effects.
 **/

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

/**
 * The removeMP3 method takes in three parameters of type mp3 which holds the address 
 * of a pointer. This method will then check the location of the node to be removed and removes it from
 * the list. Then it will free the space that node allocated for the artist, song name, and 
 * the node itself. There are not side effects.
 **/

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

/**
 * The forwardPrinting method takes in one parameter of type mp3 which holds address of 
 * the first node. This method will then check to see if list is empty or not and will
 * print the list forwards. There are no side effects.
 * 
 **/

void forwardPrinting(mp3_t *first){
  if (first == NULL) { 
      printf("There is no MP3 data in the List\n");
  }
  else{
    printf("This is MP3 data in the order it was added (Forward):\n");
    while(first != NULL){
      printf("The Artist is %s, ", first->artist);
      printf("The Song title is %s, ", first->songName);
      printf("The Year is %d, ", first->year);
      printf("and the Duration is %d minutes. \n", first->runtime);
      first=first->next;
    }
  }
}

/**
 * The reversePrinting method takes in one parameter of type mp3 which holds address of 
 * the tail node. This method will then check to see if list is empty or not and will
 * print the list in reverse. There are no side effects.
 **/

void reversePrinting(mp3_t *tail){
  if(tail == NULL){ 
    printf("There is no MP3 data in the List\n");
  }
  else{
    printf("This is the MP3 data by the most recent addition (Reverse):\n");
    while(tail != NULL){
      printf("The Artist is %s, ", tail->artist);
      printf("The Song title is %s, ", tail->songName);
      printf("The Year is %d, ", tail->year);
      printf("and the Duration is %d minutes. \n", tail->runtime);
      tail = tail->prev;
    }
  }
}

/**
 * The clearHeap method takes in one parameter of type mp3 which holds address of 
 * the first node. This method will then traverse the list and free the space that 
 * was allocated for each node and its artist and song title character arrays. 
 * There are no side effects.
 **/

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

/**
 * This is the main where the first and tail mp3 nodes are created and set to NULL. 
 * Then it creates an option screen with 5 choices allowing the user to continuously enter data, 
 * remove data, print forward and print in reverse. Then after the option screen is exited then it 
 * calls the clearHeap method to free the allocated space.
 **/

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
    } while(choices != '5');
      clearHeap(first);
	return 0;
}