#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_IDENT		10		//length of identifier for node	
#define MAX_ITEMS		256		//max number of rows in the input data 
#define MAX_COSTS		256 		//max costs for one link
#define MAX_NODE_ID		256		//max costs for one node
#define MAX_LINE		256		//max chars in one line
#define NODE_ID_INIT	-1			//init of the node id

//structure for link
typedef struct{
	int costs;					
								
}link;

//Datastructure for one node 
typedef struct{
	char name[MAX_IDENT+1];				
	int nodeID;					
	link *plinks;					
	
	int nextHop;					
    	int rootID;					
    	int sumCosts;					
	int msgCnt;					
	#ifdef DEBUG
    int changerootCnt;         			
	#endif
} node;

typedef enum linetype { typeNode, typeCommand_blank, typeLink, typeGraph_begin, typeGraph_end, typeUndefined } linetype;

//function prototypes
void sptree(int index, int nodeCnt);

//Build Graph
int getGraph(FILE *fp);
int appendNode(char *name, int nodeCnt);
void appendLink(int nodeCnt);

//validating
int checkGraph(int nodeCnt);
linetype checkLine(char *line);
int isValid(char *string);
int getNeighbourRoot(int index);
int getNeighbourCoststoRoot(int index);
int checkIfConnected(int nodeCnt);
int checkPaths(char *name, int nodeCnt, int *list);

//helper function
int getIndex(char *name, int nodeCnt);
int getNameFromID(int id, char *name, int nodeCnt);
int getID(int index);
void getName(char *name, int index);
int fillList(int *list, int id, int nodeCnt);

//output
void printTable(int nodeCnt);
void printTreeResult(int nodeCnt);

int isTreeFinish(int nodeCnt);
