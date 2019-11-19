struct item{
	int item;
	struct item *prox;
};
typedef struct item Item;

typedef struct{
    Item *inicio;
    Item *fim;
} Lista;

// #1)
void makeEmptyList (Lista *l);
// #2)
void freeList (Lista *l);
// #3)
void addItemEnd (Lista *l, int item);
// #4)
Item *findItem (Lista *l, int itemToFind);