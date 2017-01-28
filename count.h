#ifndef count_h
#define count_h

//defines a linked list structure for the hashtable
typedef struct node
{
    long long int val;
    struct node * next;
}node_t;

int isEmpty(FILE *file);
void hashInsert(node_t* m[],long long int ins,int hlong,int hashValue);
node_t** rehash(node_t** tab);
void insert(long long int ins,int hlong,int hashValue);
void freeUp(node_t** f);


#endif /* third_h */