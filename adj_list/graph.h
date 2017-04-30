
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct adjListNode {
  int id;
  int isLock;
  struct adjListNode* nextNode;
};

struct nodeList {
  struct adjListNode* headNode;
  struct nodeList* nextList;
};

struct AdjList {
  struct nodeList* head;
};

struct linkedlist {
  struct adjListNode* linkHead;
};


void push(int pid, int isLock, struct linkedlist* list) {

  struct adjListNode* newNode = malloc(sizeof(struct adjListNode));
  newNode->id = pid;
  newNode->isLock = isLock;
  newNode->nextNode = list->linkHead;
  list->linkHead = newNode;
}


void pull(int pid, int isLock, struct linkedlist* list) {
  struct adjListNode* currNode = list->linkHead;
  struct adjListNode* prevNode;

  while(currNode != NULL) {
    //found the node we want to remove
    if(currNode->id == pid && currNode->isLock == isLock) {
      //if the node we want to remove and because prev is null it was the head
      if(prevNode == NULL) {
        list->linkHead = currNode->nextNode;
      } else { //If the remove node is not the head we will always have a prev
        prevNode->nextNode = currNode->nextNode;
      }
      free(currNode);
      return;
    }
    prevNode = currNode;
    currNode = currNode->nextNode;
  }
}

void printList(struct linkedlist* list){
  struct adjListNode* currNode = list->linkHead;
  int currID;
  while(currNode != NULL) {
    currID = currNode->id;
    printf("%d\n",currID);
  }
}


void rag_request(int pid, int lockid);
void removeReqEdge(int pid, int lockid);
void addNodeToList(struct nodeList *curr_node_list, int new_node_id, int isLock);
void addNodeList(int head_node_id, int isLock);
