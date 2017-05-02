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

void rag_request(int pid, int lockid);
void removeReqEdge(int pid, int lockid);
void addNodeToList(struct nodeList *curr_node_list, int new_node_id, int isLock);
void addNodeList(int head_node_id, int isLock);
struct nodeList* getAdjList(struct adjListNode* nodeToFind);
int recursive_deadlock_detect();
void printNode(int pid, int isLock);

// int NLOCK = 10;
// int NPROC = 20;

extern struct AdjList *A;
extern struct linkedlist *whiteList, *greyList, *blackList; // helper list for deadlock detection
