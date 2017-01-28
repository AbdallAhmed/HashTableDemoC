#include <stdio.h>
#include <stdlib.h>
#include "count.h"

int count = 0;
//load factor can be changed
double lf = .75;
int t_len = 0;
node_t** ht;

//void insert(long long int ins,int hlong,int hashValue);
//void freeUp(node_t** f);

//checks to see if the file is empty
int isEmpty(FILE *file)
{
    long savedOffset = ftell(file);
    fseek(file, 0, SEEK_END);

    if (ftell(file) == 0)
    {
        return 0;
    }

    fseek(file, savedOffset, SEEK_SET);
    return 1;
}

//special insertion method just in the case of rehash
void hashInsert(node_t* m[],long long int ins,int hlong,int hashValue)
{

	//checks to see if the linked list in the hashtable at that location is empty
	//if it is then insert as the new head
	if(m[hashValue] == NULL)
	{
		node_t* new_node;
    	new_node = malloc(sizeof(node_t));
    	new_node->val = ins;
    	new_node->next = NULL;
    	m[hashValue] = new_node;
	}
	
	else
	{
		//if the list is nonempty then run through it
		node_t * check = m[hashValue];
		node_t * prev = NULL;
		//printf("Value of m[%d]: %lld\n",hashValue,check->val);
		//printf("Value wanting to insert %lld\n",ins);

		//do not need to do a duplicate check as these are just rehashed values
		//must all be unique
		//moves our ptr to the end of the list where we need to insert
		do
		{
			prev = check;
			check=check->next;
		}while(check!=NULL);

		//inserts at the end of a list 
		node_t* new_node;
	    new_node = malloc(sizeof(node_t));
		new_node->val = ins;
	   	new_node->next = NULL;
	    prev->next = new_node;
	}

	
	//printf("%f\n",loadfact);
}

//rehashing functionality 
node_t** rehash(node_t** tab)
{
	//saves the old length of our hashtable
	int old_tlen = t_len;

	//doubles the size of our table
	t_len = t_len*2;

	//create a new table that is twice the length of the new one
	//this means that for an instance of this program there exists two tables
	node_t **new_ht = (node_t **) malloc(t_len * sizeof(node_t *));

	int i;

	//initialize values from the malloc call
	for(i=0; i<t_len; i++)
	{
		new_ht[i] = NULL;
	}

	//iterates over the old table
	for(i=0; i<old_tlen; i++)
	{
		//if the old table is null at this index value, continue forward
		if(tab[i] == NULL)
			continue;
		else 
		{
			node_t * tmp = tab[i];
			do
			{
				//go through ever linked list and see where the values need to be inserted
				//the insertion area can change
				int hVal = tmp->val % t_len;
				hashInsert(new_ht,tmp->val,t_len,hVal);
				tmp = tmp->next;
			}while(tmp!=NULL);

		}
	}

	return new_ht;
}

void insert(long long int ins,int hlong,int hashValue)
{
	//if the linked list in the hash table at the index of the hash value is null make a new list
	if(ht[hashValue] == NULL)
	{
		node_t* new_node;
    	new_node = malloc(sizeof(node_t));
    	new_node->val = ins;
    	new_node->next = NULL;
    	ht[hashValue] = new_node;
    	//count is checking to see how many unique values are being inserted
    	count++;
	}
	
	else
	{
		//do not want to insert duplicate values (trying to count unique)
		node_t * check = ht[hashValue];
		node_t * prev = NULL;

		do
		{
			//exit if the value you have already exists in the table
			if(check->val == ins)
				return;
			else 
			{
				prev = check;
				check=check->next;
			}

		}while(check!=NULL);

		//create a new node and insert it at the end of the list if needed
		node_t* new_node;
	    new_node = malloc(sizeof(node_t));
	    new_node->val = ins;
	    new_node->next = NULL;
	    prev->next = new_node;

	    //count is checking to see how many unique values are being inserted
	    count++;
	}

	//calculate the load factor of the table
	double loadfact = ((double)(count))/hlong;
	
	//checks what the load factor of the hashtable is and sees if it must rehash
	if(loadfact >= lf)
	{
		//creates new table
		node_t **new_ht;
		
		//makes the value of this table equal to the result of rehashing the old one
		new_ht = rehash(ht);

		//frees the memory used  old table
		freeUp(ht);

		//reassigns our global ptr from old table to new table
		ht = new_ht;
		
	}
}

//since we have an instance in our program where two tables exist we use this function to free
//the table that we are no longer interested in 
void freeUp(node_t** f)
{
	int x;
	//goes over the entire old table size
	for(x=0; x<(t_len/2); x++)
	{
		//if the current index value is empty, do nothing
		if(f[x] == NULL)
			continue;
		else
		{	
			//if there is a list, free each node
			node_t* tmp = f[x];
			node_t* hold = NULL;
			while(tmp != NULL)
    		{
    			hold = tmp->next;
    			free(tmp);
    			tmp = hold;
    		}
		}
	}

	free(f);
}

int main(int argc, char* argv[])
{
    FILE * fpointer;

    //makes sure the program received the correct # or arguments
  	if(argc != 2)
    {
    	printf("error\n");
    	exit(1);
    }

    //if the file trying to be opened is not valid, program exits
    if((fpointer = fopen(argv[argc-1],"r")) == NULL)
    {
    	printf("error\n");
    	exit(1);
    }

	//if the file is empty, print 0 and end
	if(isEmpty(fpointer) == 0)
    {
    	printf("0\n");
    	return 0;
    }

    //this is the starting table size
	t_len = 1000;

	//mallocs enough room for our initial hashtable
 	ht = (node_t **) malloc(t_len * sizeof(node_t *));
    int x;

    //initialize the values from our malloc call
    for(x=0; x<1000; x++)
    {
    	ht[x] = NULL;
    }

  
  	long long int hex;

  	//loops through the file to get the new addresses
	while(!feof(fpointer))
	{
		fscanf(fpointer, "%llx \n", &hex);

		//very simple hashvalue which is the memory address modded by table size
		int hV = hex % t_len;

		insert(hex,t_len,hV);
		//sprintf("%lld : %lld \n", hex,hex%1000);
		//printf("%lld\n", hex);
	}

	printf("%d\n", count);
	
	//goes through the entire hashtable and frees each linked list
	int j;
	for(j=0; j<t_len; j++)
	{
		if(ht[j] == NULL)
			continue;
		else
		{
			node_t* tmp = ht[j];
			node_t* hold = NULL;
			while(tmp != NULL)
    		{
    			hold = tmp->next;
    			free(tmp);
    			tmp = hold;
    		}
		}
	}
	free(ht);
	fclose(fpointer);
	return 0;
}
