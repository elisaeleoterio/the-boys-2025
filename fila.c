/*
 * fila.c
 * Funções que implementam o tipo abstrato de dados FILA (sem prioridade).
 * Autor: Elisa R. Eleotério
 * Data: 10/06/2025
 */

#include <stdio.h>
#include <stdlib.h>
#include "entidades.h"
#include "fila.h"


// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fila_t *fila_cria () {
    struct fila_t *f;
    f = malloc(sizeof(struct fila_t));
    if (f == NULL)
        return NULL;
    f->prim = NULL;
    f->num = 0;
    return f;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fila_t *fila_destroi (struct fila_t *f) {
    if (f == NULL)
        return NULL;
    struct fila_nodo_t *temp;
    struct fila_nodo_t *aux = f->prim;
    while (aux != NULL) {
        temp = aux->prox;
        free(aux);
        aux = temp;
    }
    free(f);
    f = NULL;
    return NULL;
}

// Insere o item na fila, mantendo-a ordenada.
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fila_insere (struct fila_t *f, void *item) {
    if (f == NULL || item == NULL) {
        return -1;
    }
        
    struct fila_nodo_t *temp;
    temp = f->prim;
    
    // Verifica se esse item já existe na linha
    while (temp) {
        if ((temp->item == item)) {
            return -1;
        }
        temp = temp->prox;
    }

    struct fila_nodo_t *novo_elem = malloc(sizeof(struct fila_nodo_t));
    if (novo_elem == NULL)
        return -1;
    
    // O auxiliar vai receber os valores da função
    novo_elem->item = item;
    
    // colocar o novo elemento no fim da fila    
    novo_elem->prox = NULL;
    if (f->prim == NULL) {
        f->prim = novo_elem;
    } else {
        temp = f->prim;
        while (temp->prox) {
            temp = temp->prox;
        }
        temp->prox = novo_elem;
    }
    (f->num)++;
    return f->num;
}


// Retira o primeiro item da fila
void fila_retira (struct fila_t *f) {
    
    if ((f == NULL) || (f->prim == NULL))
        return;

    struct fila_nodo_t *aux = f->prim;

    // Primeiro atualizado
    f->prim = f->prim->prox;
    
    // Diminuí o tamanho da lista
    (f->num)--;

    free(aux);
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fila_tamanho (struct fila_t *f) {
    if ((f == NULL))
        return -1;
    return f->num;
}

// Imprime o conteúdo da fila no formato "(ID) (ID) ..."
void fila_imprime (struct fila_t *f) {
    if ((f == NULL) || (f->prim == NULL)) {
        return;
    }
    

    struct fila_nodo_t *atual;
    atual = f->prim;
    for (int i = 0; i < f->num; i++) {
        printf("(%d)", ((struct heroi_t *)atual->item)->id);
        if (i < f->num - 1)
            printf(" ");
        atual = atual->prox;
    }
}
