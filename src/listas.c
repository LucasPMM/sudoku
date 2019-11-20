#include <stdio.h>
#include "../includes/listas.h"
#include <stdlib.h>
#include <string.h>

void makeEmptyList (Lista *l) {
    l->inicio = (Item*) malloc(sizeof(Item));
    l->fim = l->inicio;
    l->size = 0;
    l->inicio->prox = NULL;
}

void freeList (Lista *l) {
    Item *item = l->inicio;
    while (item != NULL) {
        l->inicio = item->prox;
        free(item);
        item = l->inicio;
    }
}

void addItemOrdered(Lista *l, int item) {
    Item *item_before = l->inicio;
    int i;
    for (i = 0; i < l->size; i++) {
        if (item < item_before->prox->item) {
            break;
        }
        item_before = item_before->prox;
    }
    Item *new_item = (Item *) malloc(sizeof(Item));
    new_item->item = item;
    new_item->prox = item_before->prox;
    item_before->prox = new_item;

    if(item_before == l->fim)
        l->fim = new_item;
}

void addItemEnd (Lista *l, int item) {
    l->fim->prox = (Item*) malloc(sizeof(Item));
    l->fim = l->fim->prox;
    l->fim->item = item;
    l->size++;
    l->fim->prox = NULL;
}

Item *findItem (Lista *l, int itemToFind) {
    Item *item = l->inicio;
    while (item->prox != NULL) {
        if (item->prox->item == itemToFind) {
            return item->prox;
        }
        item = item->prox;
    }
    return NULL;
}

Item *findItemSet (Lista *l, int itemToFind) {
    Item *item = l->inicio;
    while (item->prox != NULL) {
        if (item->prox->item == itemToFind) {
            return item->prox;
        }
        item = item->prox;
    }
    return l->fim;
}