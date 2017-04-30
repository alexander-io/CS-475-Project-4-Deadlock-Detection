
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

// free node list
void freeNodeList(struct nodeList* nlist) {
  struct adjListNode* toFree = nlist->headNode;
  if (toFree == NULL) return;

  struct adjListNode* nextNode = toFree->nextNode;

  if(nextNode == NULL) free(toFree);

  while (nextNode != NULL) {
    free(toFree);
    toFree = nextNode;
    nextNode = toFree->nextNode;
  }
}

// free adj list
void freeAdjList(struct AdjList* alist){
  struct nodeList* toFree = alist->head;

  if (toFree == NULL)return;

  struct nodeList* secondList = toFree->nextList;

  if (secondList == NULL){
    freeNodeList(toFree);
    free(toFree);
  }

  while (secondList!=NULL){
    freeNodeList(toFree);
    free(toFree);
    toFree = secondList;
    secondList = toFree->nextList;
  }
}

// free linked list
void freeLinkedList(struct linkedlist* list) {
  struct adjListNode* toFree = list->linkHead;
  if (toFree == NULL) return;

  struct adjListNode* nextNode = toFree->nextNode;

  if(nextNode == NULL) free(toFree);

  while (nextNode != NULL) {
    free(toFree);
    toFree = nextNode;
    nextNode = toFree->nextNode;
  }
}

// push
void push(int pid, int isLock, struct linkedlist* list) {
  struct adjListNode* newNode = malloc(sizeof(struct adjListNode));
  newNode->id = pid;
  newNode->isLock = isLock;
  newNode->nextNode = list->linkHead;
  list->linkHead = newNode;
}

// contains
int contains(struct adjListNode* searchNode, struct linkedlist* list) {
  struct adjListNode* currNode = list->linkHead;
  while(currNode != NULL) {
    if(currNode->id == searchNode->id && currNode->isLock == searchNode->isLock) {
      return 1;
    }
    currNode = currNode->nextNode;
  }
  return 0;
}

// pull
struct adjListNode* pull(struct adjListNode* searchNode, struct linkedlist* list) {
  struct adjListNode* currNode = list->linkHead;
  struct adjListNode* prevNode = NULL;

  while(currNode != NULL) {
    //found the node we want to remove
    if(currNode->id == searchNode->id && currNode->isLock == searchNode->isLock) {
      //if the node we want to remove and because prev is null it was the head
      if(prevNode == NULL) {
        list->linkHead = currNode->nextNode;
      } else { //If the remove node is not the head we will always have a prev
        prevNode->nextNode = currNode->nextNode;
      }
      return currNode;
    }
    prevNode = currNode;
    currNode = currNode->nextNode;
  }
  return NULL;//the node was not found return false
}

// printList
void printList(struct linkedlist* list){
  struct adjListNode* currNode = list->linkHead;
  if(currNode == NULL) {
    printf("head null\n");
    return;
  }
  int currID;
  while(currNode != NULL) {
    currID = currNode->id;
    printf("pl %d\n",currID);
    currNode = currNode->nextNode;
  }
}

void rag_request(int pid, int lockid);
void removeReqEdge(int pid, int lockid);
void addNodeToList(struct nodeList *curr_node_list, int new_node_id, int isLock);
void addNodeList(int head_node_id, int isLock);
struct nodeList* getAdjList(struct adjListNode* nodeToFind);
int recursive_deadlock_detect();
void printNode(int pid, int isLock);
