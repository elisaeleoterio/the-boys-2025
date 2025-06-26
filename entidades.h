/*
 * entidades.h
 * Protótipos das funções que implementam as entidades do projeto The Boys.
 * Autor: Elisa R. Eleotério
 * Data: 10/06/2025
 */

#ifndef ENTIDADES_H
#define ENTIDADES_H

#include "fprio.h"
#include "conjunto.h"
#include "fila.h"

// definições para o mundo
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 5)
#define N_MISSOES (T_FIM_DO_MUNDO / 100)
#define N_COMPOSTOS_V (N_HABILIDADES * 3)

// Descreve o tipo coordenadas
struct coord_t {
  int x;
  int y;
};

// Estrutura para armazenar a distância associado a um id de um base
struct dist_t {
  int distancia;
  int idBase;
};

// Descreve uma entidade do tipo HERÓI
struct heroi_t {
  int id;
  int xp;
  int paciencia;
  struct cjto_t *habilidades;
  int velocidade;
  int base;
  int estado;
};

//Descreve uma entidade do tipo BASE
struct base_t {
  int id;
  int lotacao;
  struct cjto_t *presentes;
  struct fila_t *espera;
  struct coord_t *local;
  int missoes;
  int tamEsperaMax;
};

//Descreve uma entidade do tipo MISSÂO
struct missao_t {
  int id;
  struct coord_t *local;
  struct cjto_t *habilidades;
  int feita;
  int tentativas;
};

// Descreve uma entidade do tipo MUNDO
struct mundo_t {
  int NHerois;
  struct heroi_t **herois;
  int NBases;
  struct base_t **bases;
  int NMissoes;
  struct missao_t **missoes;
  int NHabilidades;
  struct coord_t *TamMundo;
  int Relogio;
  int CompostosV;
};

struct evento_t {
  unsigned int tempo;
  unsigned int tipo;
  unsigned int primDado; // id do heroi ou da missao
  unsigned int segDado; // id da base
};



// Protótipos das funções de CRIAÇÃO:

// Cria o mundo e define algumas características
// retorna um ponteiro para o mundo
struct mundo_t *criarMundo();

// cria um herói e incializa suas características
// retorna um ponteiro para esse heroi
struct heroi_t *criarHeroi(unsigned int idH);

// cria uma base e incializa suas características
// retorna um ponteiro para essa base
struct base_t *criarBase(unsigned int idB);

// cria uma missão e incializa suas características
// retorna um ponteiro para essa missão
struct missao_t *criarMissao(unsigned int idM);

// Protótipos das funções de INICIALIZAÇÃO:

// cria um vetor com todos os heróis do mundo
struct heroi_t **iniciaHerois(int numHerois);

// cria um vetor com todas as bases do mundo
struct base_t **iniciaBases(int numBases);

// cria um vetor para armazenar as missoes do mundo
struct missao_t **iniciaMissao(int numMissoes);

// destroi o mundo
struct mundo_t *destroiMundo(struct mundo_t *mundo);

// Protótipos dos EVENTOS:
// destoi um evento passado como parâmetro e devolve qual era o seu tipo
int destroiEvento(struct evento_t *evento);

// cria um struct para armazenar os dados do evento e o retorna
struct evento_t *criaEvento(int tempo, int tipo, int primDado, int segDado);

// sorteia um valor aleatório entre min e max e o retorna
int aleat(int min, int max);

//Representa um herói H chegando em uma base B no instante T. 
//Ao chegar, o herói analisa o tamanho da fila e decide se espera 
//para entrar ou desiste.
void chega(struct fprio_t *lef, unsigned int tempo, struct heroi_t *heroi, struct base_t *base);

// O herói H entra na fila de espera da base B.
// Assim que H entrar na fila, o porteiro da base B deve 
//ser avisado para verificar a fila
void espera(struct fprio_t *lef, unsigned int tempo, struct heroi_t *heroi, struct base_t *base);

// O herói H desiste de entrar na base B, 
//escolhe uma base aleatória D e viaja para lá
void desiste(struct fprio_t *lef, unsigned int tempo, struct heroi_t *heroi, struct base_t *base);

// O porteiro da base B trata a fila de espera
void avisa(struct fprio_t *lef, unsigned int tempo, struct base_t *base);

// O herói H entra na base B. Ao entrar, o herói 
//decide quanto tempo vai ficar e agenda sua saída da base
void entra(struct fprio_t *lef, unsigned int tempo, struct heroi_t *heroi, struct base_t *base);

// O herói H sai da base B. Ao sair, escolhe uma base de destino para viajar;
// o porteiro de B é avisado, pois uma vaga foi liberada. 
void sai(struct fprio_t *lef, unsigned int tempo, struct heroi_t *heroi, struct base_t *base);

// O herói H se desloca para uma base D (que pode ser a mesma onde já está)
void viaja(struct fprio_t *lef, unsigned int tempo, struct heroi_t *heroi, struct base_t *baseD, struct mundo_t *m);

// O herói H morre no instante T.
// O herói é retirado da base B e libera uma vaga na base.
// O porteiro de B deve ser avisado da nova vaga.
// Eventos futuros de um herói morto devem ser ignorados.
void morre(struct fprio_t *lef, unsigned int tempo, struct heroi_t *heroi, struct missao_t *missao, struct mundo_t *mundo);

//  Uma missão M é disparada no instante T.
// Cada missão ocorre em um local aleatório e requer um conjunto 
// aleatório de habilidades; ambos são definidos durante a inicialização.
// Cada equipe é formada pelo conjunto de heróis presentes em uma base.
// Uma equipe está apta para a missão se a união das habilidades de 
// seus heróis contém as habilidades requeridas pela missão.
// Deve ser escolhida para a missão a equipe da base mais 
// próxima ao local da missão e que esteja apta para ela.
// Ao completar uma missão, os heróis da equipe 
// escolhida ganham pontos de experiência.
// Um herói pode morrer ao participar de uma missão.
// Se nenhuma equipe estiver apta para a miss˜ao, ´e poss´ıvel que o her´oi mais
// experiente da base mais pr´oxima utilize um Composto V, se dispon´ıvel.
// O Composto V far´a com que ele consiga todas as habilidades necess´arias.
// Contudo, ap´os a utiliza¸c˜ao do Composto V, o her´oi morre
// Se uma missão não puder ser completada, ela é marcada como 
// “impossível” e adiada de 24 horas.
void missao(struct fprio_t *lef, unsigned int tempo, struct missao_t *m, struct mundo_t *mundo);

// Encerra a simulação no instante T
void fim(unsigned int tempo, struct fprio_t *lef, struct mundo_t *m, int contadorEventos);

#endif