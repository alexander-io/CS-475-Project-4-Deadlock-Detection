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

  // traverse the adj-list if node-lists exist
  while (curr_node_list != NULL){

    // set current node to head of list
    curr_adj_node = curr_node_list->headNode;

    // compare the pid-parmakeameter with the curr_adj_node->id
    // if we've found a match, traverse the list
    if (pid == curr_adj_node->id && !curr_adj_node->isLock){
      // add node to the list & return

      addNodeToList(curr_node_list, lockid, 1);
      return;
    }
    // move to the next list
    curr_node_list = curr_node_list->nextList;
  }

  // if there isn't a head
  // also accomodate if we loop through and can't find a matching process
  // printf("%s\n", );
  addNodeList(pid, 0);
  addNodeToList(A->head, lockid, 1);
}


// add allocation edge from lockid to pid
// remove request edge from pid to lockid
void rag_alloc(int pid, int lockid) {

  // set temporary "head" variables so we can traverse
  struct nodeList *curr_node_list = A->head;
  struct adjListNode *curr_adj_node;

  // traverse the adj-list if node-lists exist
  while (curr_node_list != NULL){

    // set current node to head of list
    curr_adj_node = curr_node_list->headNode;

    // compare the pid-parameter with the curr_adj_node->id
    // if we've found a match, traverse the list
    if (lockid == curr_adj_node->id && curr_adj_node->isLock){
      // add node to the list & return
      addNodeToList(curr_node_list, lockid, 0);
      return;
    }
    // move to the next list
    curr_node_list = curr_node_list->nextList;
  }

  // if there isn't a head
  addNodeList(lockid, 1);
  addNodeToList(A->head, pid, 0);


  // remove request edge from pid to lockid
  removeReqEdge(pid, lockid);
}

void rag_dealloc(int pid, int lockid) {

}

void rag_print() {

  // printf("print() a head id : %d\n", A->head->headNode->id);
  // if (A->head->nextList!=NULL)
  //   printf("print() head next id : %d\n", A->head->nextList->headNode->id);


  struct nodeList *currL = A->head;
  // struct adjListNode *currN = A->head->headNode;
  struct adjListNode *currN;

  while(currL!=NULL) {
    currN = currL->headNode;
    ///////
    while(currN!=NULL) {

      if (currN->isLock)
        printf("lockid=%d ", currN->id);
      else
        printf("pid=%d ", currN->id);

      currN = currN->nextNode;

    }
    ////////////
    printf("\n");
    // printf("print2() a head id : %d\n", A->head->headNode->id);
    // printf("print2() a head id : %d\n", currL->headNode->id);
    //
    // if (currL->nextList!=NULL)
    //   // printf("print2() head next id : %d\n", A->head->nextList->headNode->id);
    //   printf("print2() head next id : %d\n", currL->nextList->headNode->id);

    currL = currL->nextList;
  }
}

void deadlock_detect(void) {

}

// init adj list
void initAdjList(){
  A = malloc(sizeof(struct AdjList));
}

// make a new list
// make a new node
// add new node to head of new list
void addNodeList(int head_node_id, int isLock){
  struct nodeList *new_list = malloc(sizeof(struct nodeList));
  struct adjListNode *new_node = malloc(sizeof(struct adjListNode));

  // set properties of the new node, from parameters
  new_node->id = head_node_id;
  new_node->isLock = isLock;

  // set the head of the new list to be the new node
  new_list->headNode = new_node;
  // printf("~~~~~~~~~\n");
  // rag_print();
  // link the new head and the old head

  struct nodeList *nl = A->head;
  A->head = new_list;
  new_list->nextList = nl;

  // new_list->nextList = A->head;

  // new_list->nextList = &(A->head);

  // A->head->nextList = &nl;
  // printf("|||||||||||");
  // rag_print();

  printf("a head id : %d\n", A->head->headNode->id);
  if (A->head->nextList!=NULL)
    printf("head next id : %d\n", A->head->nextList->headNode->id);

}

// add node
void addNodeToList(struct nodeList *curr_node_list, int new_node_id, int isLock){
  // malloc size for node, set its variables
  struct adjListNode *insert_node = malloc(sizeof(struct adjListNode));
  insert_node->id = new_node_id;
  insert_node->isLock = isLock;
  // // unlink head-> next_node, place in head->insert->next_node
  insert_node->nextNode = curr_node_list->headNode->nextNode;
  curr_node_list->headNode->nextNode = insert_node;

  printf("tolist() a head id : %d\n", A->head->headNode->id);
  if (A->head->nextList!=NULL)
    printf("tolist() head next id : %d\n", A->head->nextList->headNode->id);

  // struct adjListNode *print_node = curr_node_list->headNode;
  // while (print_node!=NULL){
  //   printf("id : %d\n", print_node->id);
  //   print_node = print_node->nextNode;
  // }

}

// remove request edge from pid->lockid
void removeReqEdge(int pid, int lockid){
  struct nodeList *curr_list = A->head;
  struct adjListNode *curr_node;

  // loop through the lists
  while (curr_list!=NULL) {
    // if we've found a list with a maching headnode->id
    if (curr_list->headNode->id == pid){

      // set the current node to the head of the list
      curr_node = curr_list->headNode;

      // loop through node in list
      while (curr_node->nextNode!=NULL) {

        // if the next node is a match, remove the next node
        if (curr_node->nextNode->id == lockid) {
          struct adjListNode *remove_node = curr_node->nextNode;
          // this is the node we want to remove
          curr_node->nextNode = curr_node->nextNode->nextNode;
          free(remove_node);
        }
        // move on to check the next node
        curr_node = curr_node->nextNode;
      }
    }
    // move to the next list
    curr_list = curr_list->nextList;
  }
}


// take in the instruction as a paremeter, call the appropriate functions :
// rag_request
// rag_alloc
// rag_dealloc
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
  // initialize the adj list
  initAdjList();

  // read in file
  FILE *file = fopen("./input.txt","r");
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
          // req = token;
          break;

        case 2:
          lockid = atoi(token);
          break;

      }
      ++counter;
      // printf("%s\n", token);
      token = strtok(NULL, ",\n");
    }
    counter=0;

    // printf("%d,%c,%d\n", pid, (*req), lockid);

    reqFind(pid, (*req), lockid);
  }
  rag_print();
  deadlock_detect();
}
