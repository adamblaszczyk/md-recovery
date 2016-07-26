/*
 *  -----------------------------------------
 *  Message Digest Recovery 1.00
 *  -----------------------------------------
 *  Author: Adam Blaszczyk
 *          http://wyciekpamieci.blogspot.com
 *  Date:   2016-07-26
 *  -----------------------------------------
 *  
 *  Compilation (Linux, Windows):
 *         gcc mdrec.c -o mdrec -lcrypto
 *
 *  Libs (Linux):
 *         sudo apt-get install libssl-dev
 *
 */

#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>

#define HASH_SIZE 33
#define DICT_SIZE 255

void title() {
  printf("\n");
  printf("+--------------------------------+\n");
  printf("|  MESSAGE DIGEST RECOVERY 1.00  |\n");
  printf("+--------------------------------+\n");
  printf("\n");
}

void md5sum(char* string, char mdString[33]) {

  unsigned char digest[MD5_DIGEST_LENGTH];

  MD5((unsigned char*) string, strlen(string), (unsigned char*)&digest);

  int i;
  for(i = 0; i < 16; i++)
    sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
}


int main() {

  FILE *fhash, *fdict, *fresult;
  char str[255];
  char hash[256][HASH_SIZE];
  char dict[DICT_SIZE];
  int i, j;
  char h[33];
  char res[255];
  int counter = 0;
  
  title();
  
  //---DICTIONARY--------------------------------------------------------------
  printf("Enter hash file name: ");
  gets(str);
  fhash = fopen(str, "r");
  if(fhash==NULL) {
    printf(">>> File not found!\n");
    return 0;
  }
  i = 0;
  while(!feof(fhash)) {
    fscanf(fhash, "%s", hash[i]);
    i++;
  }
  fclose(fhash);
  
  printf("Enter dictionary file name: ");
  fflush(stdin);
  gets(str);
  fdict = fopen(str, "r");
  if(fdict==NULL) {
    printf(">>> File not found!\n");
    return 0;
  }
  
  printf("\n");
  
  for(j=0; j<i; j++) {
    rewind(fdict);
    while(!feof(fdict)) {
      fscanf(fdict, "%s", dict);
      md5sum(dict, h);
      if(!strcmp(hash[j], h)) {
        printf("%s : %s  <OK>\n", hash[j], dict);
        memset(res, 0, 255);
        strcat(res, hash[j]);
        strcat(res, " : ");
        strcat(res, dict);
        strcat(res, "\n");
        fresult = fopen("results.txt", "a");
        fputs(res, fresult);
        fclose(fresult);
        counter++;
      }
      else {
        printf("%s : %s\n", hash[j], dict);
      }
    }   
  }
      
  fclose(fdict); 
  //---------------------------------------------------------------------------
  
  printf("\n\nFound %d hashes, see results.txt", counter);
  printf("\n\nPress [Enter] to exit...\n");
  fflush(stdin);
  getchar(); 
  return 0;
}
