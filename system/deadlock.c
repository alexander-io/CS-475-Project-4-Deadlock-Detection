#include <xinu.h>

struct AdjList *A;
struct linkedlist *whiteList, *greyList, *blackList;

// free node list
void freeNodeList(struct nodeList* nlist) {
  struct adjListNode* toFree = nlist->headNode;
  if (toFree == NULL) return;

  struct adjListNode* nextNode = toFree->nextNode;

  if(nextNode == NULL) free(toFree, sizeof(toFree));

  while (nextNode != NULL) {
    free(toFree, sizeof(toFree));
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
    free(toFree, sizeof(toFree));
  }

  while (secondList!=NULL){
    freeNodeList(toFree);
    free(toFree, sizeof(toFree));
    toFree = secondList;
    secondList = toFree->nextList;
  }
}

// free linked list
void freeLinkedList(struct linkedlist* list) {
  struct adjListNode* toFree = list->linkHead;
  if (toFree == NULL) return;

  struct adjListNode* nextNode = toFree->nextNode;

  if(nextNode == NULL) free(toFree, sizeof(toFree));

  while (nextNode != NULL) {
    free(toFree, sizeof(toFree));
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
      addNodeToList(curr_node_list, pid, 0);
      removeReqEdge(pid, lockid);
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

  if(A->head == NULL) {
    kprintf("head");
  }

  struct nodeList *curr_list = A->head;
  struct adjListNode *curr_node;
  // // loop through the lists
  // while (curr_list!=NULL) {
  //   // if we've found a list with a maching headnode->id
  //   if (curr_list->headNode->id == lockid && curr_list->headNode->isLock){
  //
  //     // set the current node to the head of the list
  //     curr_node = curr_list->headNode;
  //
  //     // loop through node in list
  //     while (curr_node->nextNode!=NULL) {
  //
  //       // if the next node is a match, remove the next node
  //       if (curr_node->nextNode->id == pid) {
  //         // this is the node we want to remove
  //         struct adjListNode *remove_node = curr_node->nextNode;
  //         curr_node->nextNode = NULL; // if this is the last node, set the nxt to null
  //         // if there's another node down the list, link it up
  //         if(remove_node->nextNode!=NULL){
  //           curr_node->nextNode = remove_node->nextNode;
  //         }
  //         // remove_node
  //         free(remove_node, sizeof(remove_node));
  //         return;
  //       }
  //       // move on to check the next node
  //       curr_node = curr_node->nextNode;
  //     }
  //   }
  //   // move to the next list
  //   curr_list = curr_list->nextList;
  // }
}

/*
* print the resource allocation graph
*/
void rag_print() {
  struct nodeList *currL = A->head;
  // struct adjListNode *currN = A->head->headNode;
  struct adjListNode *currN;

  while(currL!=NULL) {
    currN = currL->headNode;
    while(currN!=NULL) {

      if (currN->isLock)
        printf("lockid=%d ", currN->id);
      else
        printf("pid=%d ", currN->id);

      currN = currN->nextNode;

    }
    printf("\n");
    currL = currL->nextList;
  }
}

// detect deadlock in the graph
void deadlock_detect(void) {
  struct nodeList *curr_list = A->head;

  //add all the head nodes into a linked list
  while(curr_list != NULL) {
    push(curr_list->headNode->id,curr_list->headNode->isLock, whiteList);
    curr_list = curr_list->nextList;
  }
  curr_list = NULL;

  //traverse the white list, for each node add it to the grey list
  struct adjListNode* currWhitelistNode = whiteList->linkHead;
  printf("-----------------------------\n");
  while(currWhitelistNode != NULL) {
    curr_list = getAdjList(currWhitelistNode);
    struct adjListNode* currNode = curr_list->headNode;

    struct adjListNode* dfs = pull(currNode,whiteList);
    push(dfs->id, dfs->isLock,greyList);

    if(recursive_deadlock_detect(dfs)){
      printf("\n");
      return;
    }

    currWhitelistNode = currWhitelistNode->nextNode;
  }

}

/*
* recursive call for deadlock detection
* @return int/boolean, 0 : no-cycle , 1 : cycle
*/
int recursive_deadlock_detect(struct adjListNode *dfsNode){
  struct nodeList* traversalNodeList = getAdjList(dfsNode);
  struct adjListNode* currNode = traversalNodeList->headNode->nextNode;
  while(currNode != NULL){
    // printf("in while\n");
    if(contains(currNode,blackList)) {

    } else if(contains(currNode,greyList)) {
      printf("DEADLOCK      ");
      printNode(currNode->id, currNode->isLock);
      return 1;
    } else {
      struct adjListNode* tmpNode = pull(currNode,whiteList);
      push(tmpNode->id, tmpNode->isLock,greyList);

      if(recursive_deadlock_detect(currNode)){
        printNode(currNode->id, currNode->isLock);
        return 1;
      }
    }
    currNode = currNode->nextNode;
  }
  struct adjListNode* tmpNode = pull(dfsNode,greyList);
  push(tmpNode->id, tmpNode->isLock,blackList);
  return 0;
}

struct nodeList* getAdjList(struct adjListNode* nodeToFind) {
  struct nodeList* curr_list = A->head;

  while(curr_list != NULL) {
    //found the node to find as in our adj list
    if(curr_list->headNode->id == nodeToFind->id && curr_list->headNode->isLock == nodeToFind->isLock){
      return curr_list;
    }
    curr_list = curr_list->nextList;
  }
  //did not find the node to find
  return NULL;
}

void printNode(int pid, int isLock) {
  if(isLock) {
    printf("lockid=%d ",pid);
  } else {
    printf("pid=%d ",pid);
  }
}

// init adj list
void initAdjList(){
  whiteList = malloc(sizeof(struct linkedlist));
  greyList = malloc(sizeof(struct linkedlist));
  blackList = malloc(sizeof(struct linkedlist));
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

  struct nodeList *nl = A->head;
  A->head = new_list;
  new_list->nextList = nl;
}

// add node
void addNodeToList(struct nodeList *curr_node_list, int new_node_id, int isLock){
  // malloc size for node, set its variables
  struct adjListNode *insert_node = malloc(sizeof(struct adjListNode));
  insert_node->id = new_node_id;
  insert_node->isLock = isLock;

  insert_node->nextNode = curr_node_list->headNode->nextNode;
  curr_node_list->headNode->nextNode = insert_node;
}

// remove request edge from pid->lockid
void removeReqEdge(int pid, int lockid){
  struct nodeList *curr_list = A->head;
  struct adjListNode *curr_node;

  // loop through the lists
  while (curr_list!=NULL) {
    // if we've found a list with a maching headnode->id
    if (curr_list->headNode->id == pid && !curr_list->headNode->isLock){

      // set the current node to the head of the list
      curr_node = curr_list->headNode;

      // loop through node in list
      while (curr_node->nextNode!=NULL) {

        // if the next node is a match, remove the next node
        if (curr_node->nextNode->id == lockid) {
          // this is the node we want to remove
          struct adjListNode *remove_node = curr_node->nextNode;
          curr_node->nextNode = NULL;
          // if there's another node down the list, link it up
          if(remove_node->nextNode!=NULL){
            curr_node->nextNode = remove_node->nextNode;
          }
          // remove_node
          free(remove_node, sizeof(remove_node));
          return;
        }
        // move on to check the next node
        curr_node = curr_node->nextNode;
      }
    }
    curr_list = curr_list->nextList; // move to the next list
  }
}


// take in the instruction as a parameter, call the appropriate functions
void reqFind(int pid, char req, int lockid) {
  switch (req) {
    case 'R':
      rag_request(pid, lockid);
      printf("pid=%d (R) requests lockid=%d\n", pid,lockid);
      break;
    case 'A':
      rag_alloc(pid, lockid);
      printf("pid=%d (A) aquires lockid=%d\n", pid,lockid);
      break;
    case 'D':
      rag_dealloc(pid, lockid);
      printf("pid=%d (D) releases lockid=%d\n", pid,lockid);
      break;
  }
}

// free global lists
void freeGlobals(){
  freeLinkedList(whiteList);
  freeLinkedList(greyList);
  freeLinkedList(blackList);
  free(whiteList, sizeof(whiteList));
  free(greyList, sizeof(greyList));
  free(blackList, sizeof(blackList));
  freeAdjList(A);
  free(A, sizeof(A));
}
