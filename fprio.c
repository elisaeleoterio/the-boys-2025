/*
 * fprio.c
 * Funções que implementam o tipo abstrato de dados FILA COM PRIORIDADE.
 * Autor: Elisa R. Eleotério
 * Data: 10/06/2025
 */

#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fprio_t *fprio_cria () {
    struct fprio_t *f;
    f = malloc(sizeof(struct fprio_t));
    if (f == NULL)
        return NULL;
    f->prim = NULL;
    f->num = 0;
    return f;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fprio_t *fprio_destroi (struct fprio_t *f) {
    if (f == NULL)
        return NULL;
    struct fpnodo_t *temp;
    struct fpnodo_t *aux = f->prim;
    while (aux != NULL) {
        temp = aux->prox;
        free(aux->item);
        free(aux);
        aux = temp;
    }
    free(f);
    f = NULL;
    return NULL;
}

// Insere o item na fila, mantendo-a ordenada por prioridades crescentes.
// Itens com a mesma prioridade devem respeitar a politica FIFO (retirar
// na ordem em que inseriu).
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio) {
    if (f == NULL || item == NULL) {
        return -1;
    }
        
    struct fpnodo_t *temp;
    temp = f->prim;
    
    // Verifica se esse item já existe na linha
    while (temp) {
        if ((temp->item == item)) {
            return -1;
        }
        temp = temp->prox;
    }

    struct fpnodo_t *novo_elem = malloc(sizeof(struct fpnodo_t));
    if (novo_elem == NULL)
        return -1;
    
    // O auxiliar vai receber os valores da função
    novo_elem->item = item;
    novo_elem->prio = prio;
    novo_elem->tipo = tipo;
    
    
    // Se a prioridade do primeiro for maior que a prioridade do novo valor ou se não houver elementos na fila
    if ((!f->num) || (prio < f->prim->prio)) {
        novo_elem->prox = f->prim;
        f->prim = novo_elem;
        (f->num)++;
        return f->num;
    }
    
    // "Resetar" o temporário
    temp = f->prim;

    // Procura o lugar na lista onde deve inserir o item
    while ((temp->prox != NULL) && (temp->prox->prio <= novo_elem->prio)) {
        temp = temp->prox;
    }
    
    novo_elem->prox = temp->prox;
    temp->prox = novo_elem;

    (f->num)++;
    return f->num;
}


// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
// do item são devolvidos nos parâmetros "tipo" e "prio".
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fprio_retira (struct fprio_t *f, int *tipo, int *prio) {
    
    if ((f == NULL) || (f->prim == NULL))
        return NULL;

    struct fpnodo_t *aux = f->prim;

    // Devolve os valores do primeiro
    void *item_old;

    item_old = f->prim->item;

    *tipo = f->prim->tipo;
    *prio = f->prim->prio;

    // Primeiro atualizado
    f->prim = f->prim->prox;
    
    // Diminuí o tamanho da lista
    (f->num)--;

    free(aux);
    
    return item_old;
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fprio_tamanho (struct fprio_t *f) {
    if ((f == NULL))
        return -1;
    return f->num;
}

// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
// Para cada item deve ser impresso seu tipo e sua prioridade, com um
// espaço entre valores, sem espaços antes ou depois e sem nova linha.
void fprio_imprime (struct fprio_t *f) {
    struct fpnodo_t *atual;
    atual = f->prim;
    for (int i = 0; i < f->num; i++) {
        printf("(%d %d)", atual->tipo, atual->prio);
        if (i < f->num - 1)
            printf(" ");
        atual = atual->prox;
    }
}
