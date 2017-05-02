#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

struct AdjList *A;
struct linkedlist *whiteList, *greyList, *blackList; // helper list for deadlock detection

int NLOCK = 10;
int NPROC = 20;


int main(int argc, char** argv) {
  // initialize the adj list
  initAdjList();

  // read in file
  FILE *file = fopen("./input4.txt","r");
  char line[21];

  int pid;
  int lockid;
  char *token;

  while(fgets(line, sizeof(line), file)) {

    char req[1];
    int counter = 0;

    token = strtok(line, ",\n");
    while (token!=NULL) {

      switch(counter){
        case 0:
          pid = atoi(token);
          break;
        case 1:
          strcpy(req, token);
          break;
        case 2:
          lockid = atoi(token);
          break;
      }
      ++counter;
      token = strtok(NULL, ",\n");
    }
    counter=0;

    reqFind(pid, (*req), lockid);
  }
  rag_print();
  deadlock_detect();
  freeGlobals();
}
