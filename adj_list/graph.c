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

    // compare the pid-parameter with the curr_adj_node->id
    // if we've found a match, traverse the list
    if (pid == curr_adj_node->id){
      // add node to the list & return
      addNodeToList(curr_node_list, lockid, 1);
      return;
    }
    // move to the next list
    curr_node_list = curr_node_list->nextList;
  }

  // if there isn't a head
  // also accomodate if we loop through and can't find a matching process
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
    if (lockid == curr_adj_node->id){
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

  // link the new head and the old head
  new_list->nextList = A->head;
  A->head = new_list;
}

// add node
void addNodeToList(struct nodeList *curr_node_list, int new_node_id, int isLock){
  // malloc size for node, set its variables
  struct adjListNode *insert_node = malloc(sizeof(struct adjListNode));
  insert_node->id = new_node_id;
  insert_node->isLock = isLock;
  // // unlink head-> next_node, place in head->insert->next_node
  insert_node->nextNode = curr_node_list->headNode->nextNode;
  // curr_node_list->headNode->nextNode = insert_node;
}

// remove request edge from pid->lockid
void removeReqEdge(int pid, int lockid){
  struct nodeList *curr_list = A->head;
  struct adjListNode *curr_node;

  while (curr_list!=NULL) {

    if (curr_list->headNode->id == pid){

      //
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
