#include "spanningtree.h"

node *pnode = NULL;
char graphname[MAX_IDENT];


int getGraph(FILE *fp)
{
   	char line[MAX_LINE];
   	char* inp;
	int type;
	char nodeA[MAX_IDENT];
	char nodeB[MAX_IDENT];
	char graphName[MAX_IDENT];
	int number, indexA, indexB;
	int nodeCnt = 0;

    while((inp = fgets(line, MAX_LINE, fp)))
    {
		type = checkLine(line);
		switch(type)
		{
			
            case typeNode:
                sscanf(line, "%s = %d", nodeA, &number);
				if(number < 0 || number > MAX_NODE_ID)
				{
					printf("Ungültiges Knotengewicht!");
					return 0;
				}
				
                if(isValid(nodeA))
                {
					if((indexA = getIndex(nodeA, nodeCnt)) != -1)
					{
						
						if(pnode[indexA].nodeID == NODE_ID_INIT)
						{
							pnode[indexA].nodeID = number;
                            pnode[indexA].rootID = number;
                            pnode[indexA].nextHop = number;
						}
					}
					else
					{
                    	nodeCnt = appendNode(nodeA, nodeCnt);
						if(nodeCnt == 0) return 0;
						pnode[nodeCnt-1].nodeID = number;
                        pnode[nodeCnt-1].rootID = number;
                        pnode[nodeCnt-1].nextHop = number;
					}
				}
				else
				{
					printf("Ungültiger Knoten!\n");
					return 0;
				}
				break;

			case typeLink:
                sscanf(line, "%s - %s : %d", nodeA, nodeB, &number);
				if(number < 0 || number > MAX_COSTS)
				{
					printf("Ungültiges Kantengewicht!");
					return 0;
				}

                if(isValid(nodeA) && isValid(nodeB))
                {
					if(getIndex(nodeA, nodeCnt) == -1)
					{
                    	nodeCnt = appendNode(nodeA, nodeCnt);
						if(nodeCnt == 0) return 0;
					}
					if(getIndex(nodeB, nodeCnt) == -1)
					{
                    	nodeCnt = appendNode(nodeB, nodeCnt);
						if(nodeCnt == 0) return 0;
					}
					
                 	indexA = getIndex(nodeA, nodeCnt);
                 	indexB = getIndex(nodeB, nodeCnt);
                 	
                 	if(pnode[indexA].plinks[indexB].costs != 0 || pnode[indexB].plinks[indexA].costs != 0)
					{
						printf("Kante doppelt definiert!\n");
						return 0;
					}
					
					
					pnode[indexA].plinks[indexB].costs = number;
                 	pnode[indexB].plinks[indexA].costs = number;
                }
				else
				{
					printf("Ungültige Knoten!\n");
					return 0;
				}
                break;
			
			case typeGraph_begin:
				
				sscanf(line, "%s %s", graphName, graphName);
				strcpy(graphname, graphName);
				break;
			
			case typeGraph_end:
				return nodeCnt;
		}
    }

    return 0;
}


linetype checkLine(char *line)
{
	
	while(isspace(*line)) line++;


	if(*line == 0 || strncmp(line, "//", 2) == 0)  return typeCommand_blank;


	if(strchr(line, '=') != 0) return typeNode;

	else if(strchr(line, ':') != 0) return typeLink;

	else if(strchr(line, '{') != 0) return typeGraph_begin;

	else if(strchr(line, '}') != 0) return typeGraph_end;

    return typeUndefined;
}


int isValid(char *string)
{
	int count = 0;

	if(isalpha(*string) == 0) return 0;
	count++;
	string++;

	while(count <= MAX_IDENT)
	{
		if(*string == '\0') return 1;
	
		if(isalpha(*string) == 0 && isdigit(*string) == 0) return 0;
		count++;
		string++;
	}
	return 0; 
}


int getIndex(char *name, int nodeCnt)
{
	for(int i = 0; i < nodeCnt; i++)
	{
		if(strcmp(pnode[i].name, name) == 0)
			return i;
	}
	
    return -1;
}


int getNameFromID(int id, char *name, int nodeCnt)
{
    for(int i = 0; i < nodeCnt; i++)
    {
        if(pnode[i].nodeID == id)
        {
            strcpy(name, pnode[i].name);
            return 1;
        }
    }
    return 0;
}


void getName(char *name, int index)
{
	strcpy(name, pnode[index].name);
}


int getID(int index)
{
	return pnode[index].nodeID;
}


int appendNode(char *name, int nodeCnt)
{
	node *tmp;
    nodeCnt++;

	tmp = (node*) realloc(pnode, nodeCnt*sizeof(node));
	if(tmp == NULL)
	{

		printf("Fehler beim realloc der Nodeliste!\n");
		free(pnode);
		return 0;
	}
	pnode = tmp;

	strcpy(pnode[nodeCnt-1].name, name);
	
	
	pnode[nodeCnt-1].nodeID = NODE_ID_INIT;
    pnode[nodeCnt-1].rootID = NODE_ID_INIT;
    pnode[nodeCnt-1].nextHop = NODE_ID_INIT;
	
	appendLink(nodeCnt);
	
	return nodeCnt;
}


void appendLink(int nodeCnt)
{
	link* tmp;
	
	pnode[nodeCnt-1].plinks = NULL;
	
	
	for(int i = 0; i < nodeCnt; i++)
	{
		tmp = (link*) realloc(pnode[i].plinks, nodeCnt*sizeof(link));
		if(tmp == NULL)
		{

			printf("Fehler beim realloc der Linkliste!\n");
			free(pnode[i].plinks);
			return;
		}
		pnode[i].plinks = tmp;
	}
	

	for(int i = 0; i < nodeCnt; i++)
	{
		pnode[i].plinks[nodeCnt-1].costs = 0;
		if(i == nodeCnt-1)
		{
			for(int j = 0; j < nodeCnt; j++)
			{
				pnode[i].plinks[j].costs = 0;
			}
		}
	}
}


int getNeighbourRoot(int index)
{
    return pnode[index].rootID;
}


int getNeighbourCoststoRoot(int index)
{
    return pnode[index].sumCosts;
}


void sptree(int index, int nodeCnt)
{
    pnode[index].msgCnt++;

    for(int i = 0; i < nodeCnt; i++)
    {
        if (pnode[index].plinks[i].costs > 0)
        {
            if (pnode[index].rootID > getNeighbourRoot(i))
            {
                pnode[index].rootID = getNeighbourRoot(i);
                pnode[index].nextHop = pnode[i].nodeID;
                pnode[index].sumCosts = getNeighbourCoststoRoot(i) + pnode[index].plinks[i].costs;
            }
            else if (pnode[index].rootID == getNeighbourRoot(i)) 
            {
                if (pnode[index].sumCosts > getNeighbourCoststoRoot(i) + pnode[index].plinks[i].costs)
                {
                    pnode[index].nextHop = pnode[i].nodeID;
                    pnode[index].sumCosts = getNeighbourCoststoRoot(i) + pnode[index].plinks[i].costs;
                }
            }
        }
    }
}


int isTreeFinish(int nodeCnt)
{
    for(int i = 0; i < nodeCnt; i++)
    {
        if(pnode[i].msgCnt < nodeCnt)
        {
            return 0;
        }
    }
    return 1;
}


int checkGraph(int nodeCnt)
{
	int rootCnt = 0;
	
	
	for(int i = 0; i < nodeCnt; i++)
	{
	
		if(pnode[i].nodeID <= 0)
		{
			printf("Node %s wurde nicht oder falsch definiert!\n", pnode[i].name);
			return 0;
		}
		if(pnode[i].nodeID == 1)
		{
			rootCnt ++;
			if(rootCnt > 1)
			{
				printf("Es wurden mehr als eine Root definiert!\n");
				return 0;
			}
		}
		for(i = 0; i < nodeCnt; i++)
		{
			for(int j = 0; j < nodeCnt; j++)
			{
				if(i == j)
				{
					if(pnode[i].plinks[j].costs != 0)
					{
						printf("Knoten %s ist mit sich selbst verbunden!\n", pnode[i].name);
						return 0;
					}
				}
			}
		}
		if(checkIfConnected(nodeCnt) == 0)
		{
			return 0;
		}
	}

    return 1;
}


int checkIfConnected(int nodeCnt)
{
	int *list = NULL;
	int *tmp = NULL;
	
	tmp = (int*) calloc(nodeCnt, sizeof(int));
	if(tmp == NULL)
	{

		printf("Fehler beim calloc der Liste!\n");
		free(pnode);
		return 0;
	}
	list = tmp;
	
	checkPaths(pnode[0].name, nodeCnt, list);
	
	for(int i = 0; i < nodeCnt; i++)
	{
		if(list[i] == 0)
		{
			printf("Der Graph ist nicht verbunden!\n");
			return 0;
		}
	}
	
	return 1;
}


int checkPaths(char *name, int nodeCnt, int *list)
{
	int index;
	char newname[MAX_IDENT];
	int id;
	
	index = getIndex(name, nodeCnt);
	
	id = getID(index);
	if(fillList(list, id, nodeCnt) > -1)
	{
		printf("added id %d to list\n", id);
	}
	else
	{
		return 0;
	}
	
	for(int i = 0; i < nodeCnt; i++)
	{		
		if(pnode[index].plinks[i].costs > 0)
		{
			getName(newname, i);
			checkPaths(newname, nodeCnt, list);
		}
	}
	
	return 0;
}


int fillList(int *list, int id, int nodeCnt)
{
	for(int i = 0; i < nodeCnt; i++)
	{
		if(list[i] == id)
		{
			return -1;
		}
		else if(list[i] == 0)
		{
			list[i] = id;
			return i;
		}
	}
	
	return -1;
}





void printTreeResult(int nodeCnt)
{
	char nodeName[MAX_IDENT+1];
	
    printf("\n\nSpanning-Tree of %s{", graphname);
	if(getNameFromID(pnode[0].rootID, nodeName, nodeCnt) == 0)
	{
		printf("Node nicht gefunden!\n");
		return;
	}
	printf("\n\tRoot: %s;", nodeName);
    for(int i = 0; i < nodeCnt; i++)
    {
		if(pnode[i].nodeID != pnode[i].rootID)
        {
			if(getNameFromID(pnode[i].nextHop, nodeName, nodeCnt) == 0)
			{
				printf("Node nicht gefunden!\n");
				return;
			}
			printf("\n\t%s - %s;", pnode[i].name, nodeName);
		}
    }
    printf("\n}\n");
	
}

