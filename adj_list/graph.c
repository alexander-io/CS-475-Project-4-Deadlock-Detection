#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

struct AdjList *A;
int NLOCK = 10;
int NPROC = 20;

// add request edge for the rag from PID to lock id
void rag_request(int pid, int lockid) {

  // set temporary "head" variables so we can traverse
  struct nodeList *curr_node_list = A->head;
  struct adjListNode *curr_adj_node;

  // traverse the lists
  while (curr_node_list != NULL){

    // set current node to head of list
    curr_adj_node = curr_node_list->headNode;

    // compare the pid-parameter with the curr_adj_node->id
    // if we've found a match, traverse the list
    if (pid == curr_adj_node->id){

      // we've found a matching process, so traverse the list & append a new lockid-node
      while (curr_adj_node != NULL){
        // do stuff
        if (curr_adj_node->nextNode == NULL){ /*if this is the last node*/
          // append the new lockid-node to the list as curr_adj_node->nextNode
          struct adjListNode *insert_node; // = {id:lockid, }
          insert_node->id = lockid;
          insert_node->isLock = 1;
          insert_node->nextNode = NULL;
          curr_adj_node->nextNode = insert_node;

          break;
        }

        // current node isn't the last, so move to the next node
        curr_adj_node = curr_adj_node->nextNode;
      }
    } else /* can't find a matching process in the graph*/{
      // append a new list to the end of the graph...
      struct nodeList *insert_list;
      curr_node_list->nextList = insert_list;
      curr_node_list->nextList->nextList = NULL;

      // create a new head node for the newly appended list
      // where the head node's id matches the pid-parameter
      struct adjListNode *insert_node; // = {id:lockid, }
      curr_node_list->nextList->headNode = insert_node;
      curr_node_list->nextList->headNode->id = pid;
      curr_node_list->nextList->headNode->isLock = 0;

      // append the lock node to the end of the list
      struct adjListNode *insert_lock_node;
      curr_node_list->nextList->headNode->nextNode = insert_lock_node;
      curr_node_list->nextList->headNode->nextNode->id = lockid;
      curr_node_list->nextList->headNode->nextNode->isLock = 1;
      curr_node_list->nextList->headNode->nextNode->nextNode = NULL;

      curr_adj_node->nextNode = insert_node;
      curr_adj_node->nextNode->id = lockid;
      curr_adj_node->nextNode->isLock = 1;
      curr_adj_node->nextNode->nextNode = NULL;
      break;

      // finally, append a new node to the end of the list...
      // where the newly appended node has an id corresponding to the lockid-parameter
    }

    // move to the next list
    curr_node_list = curr_node_list->nextList;
  }
}

void rag_alloc(int pid, int lockid) {

}

void rag_dealloc(int pid, int lockid) {

}

void rag_print() {

}

void deadlock_detect(void) {

}

void reqFind(int pid, char req, int lockid) {
  switch (req) {

    case 'R':
      rag_request(pid, lockid);
      break;
    case 'A':
      rag_alloc(pid, lockid);
      break;
    case 'D':
      rag_dealloc(pid, lockid);
      break;
  }
}

int main(int argc, char** argv) {
  A = malloc(sizeof(struct adjListNode)*(NLOCK+1) * NPROC);

  FILE *file = fopen("./input.txt","r");
  char line[7];

  int pid;
  int lockid;

  while(fgets(line, sizeof(line), file)) {
    char req;

    pid = line[0];
    req = line[2];
    lockid = line[4];



    reqFind(pid, req, lockid);
  }
  deadlock_detect();
}
