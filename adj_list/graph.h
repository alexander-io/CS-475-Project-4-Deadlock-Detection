
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


void rag_request(int pid, int lockid);
void removeReqEdge(int pid, int lockid);
void addNodeToList(struct nodeList *curr_node_list, int new_node_id, int isLock);
void addNodeList(int head_node_id, int isLock);
