#include "spanningtree.h"

int main(int argc, char *argv[])
{
	FILE *fp = stdin;
	int nodeCnt = 0;
   
   	if(argc > 2)
   	{
   		printf("Usage: %s [<file>]\n", argv[0]);
      	return 1;
   	}
   	if(argc == 2)
   	{
      	fp = fopen(argv[1], "r");
      	if(!fp)
      	{
        	printf("Could not open the file!\n");
		 	return 1;
      	}
   	}

    
	nodeCnt = getGraph(fp);
	
	
	if(checkGraph(nodeCnt) == 0)
	{
		printf("Graph nicht korrekt definiert!\n");
		return 0;
	}
	
   	if(fp!=stdin)
   	{
      	fclose(fp);
   	}

    srand((int)time(NULL));

    
    while(isTreeFinish(nodeCnt) == 0)
    {
        sptree(rand() % nodeCnt, nodeCnt);
    }

    printTreeResult(nodeCnt);

   	return 0;
}
