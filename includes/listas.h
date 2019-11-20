struct item{
	int item;
	struct item *prox;
};
typedef struct item Item;

typedef struct{
    Item *inicio;
    Item *fim;
    int size;
} Lista;

// #1)
void makeEmptyList (Lista *l);
// #2)
void freeList (Lista *l);
// #3)
void addItemEnd (Lista *l, int item);
// #4)
Item *findItem (Lista *l, int itemToFind);
// #5)
void addItemOrdered(Lista *l, int item);
Item *findItemSet (Lista *l, int itemToFind);