#include <stdio.h>
#include "../includes/listas.h"
#include <stdlib.h>
#include <string.h>

void makeEmptyList (Lista *l) {
    l->inicio = (Item*) malloc(sizeof(Item));
    l->fim = l->inicio;
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

void addItemEnd (Lista *l, int item) {
    l->fim->prox = (Item*) malloc(sizeof(Item));
    l->fim = l->fim->prox;
    l->fim->item = item;
    l->fim->prox = NULL;
}

Item *findItem (Lista *l, int itemToFind) {
    Item *item = l->inicio;
    while (item->prox != NULL) {
        if (item->prox->item == itemToFind) {
            return item;
        }
        item = item->prox;
    }
    return NULL;
}
