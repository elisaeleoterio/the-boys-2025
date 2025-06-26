/*
 * entidades.c
 * Funções que implementam as entidades do projeto The Boys.
 * Autor: Elisa R. Eleotério
 * Data: 10/06/2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "fila.h"
#include "fprio.h"
#include "entidades.h"
#include "conjunto.h"

// DEFINES:

// definições dos valores para os eventos
#define CHEGA 1
#define ESPERA 2
#define DESISTE 3
#define AVISA 4
#define ENTRA 5
#define SAI 6
#define VIAJA 7
#define MORRE 8
#define MISSAO 9

// Retorna um valor inteiro aleatório no intervalo [min, max]
int aleat(int min, int max) {
  int valor = (rand() % (max - min + 1)) + min;
  return valor;
}

// funções de CRIAÇÃO e INICIALIZAÇÃO:

// Cria dinâmicamente um herói e incializa suas características.
// Retorna um ponteiro para esse heroi.
struct heroi_t *criarHeroi(unsigned int idH) {
  
  struct heroi_t *heroi = malloc(sizeof(struct heroi_t));
  if (heroi == NULL) {
    printf("Erro ao alocar\n");
    return NULL;
  }
  
  // Astribuição das características do herói
  heroi->habilidades = cjto_aleat(aleat(1, 3), N_HABILIDADES);
  if (heroi->habilidades == NULL) {
    printf("Erro ao alocar\n");
    free(heroi);
    return NULL;
  }
  heroi->id = idH;
  heroi->xp = 0;
  heroi->paciencia = aleat(0, 100);
  heroi->velocidade = aleat(50, 5000);
  // define se está vivo (1) ou morto (0)
  heroi->estado = 1; 
  
  return heroi;
}


// Cria um vetor de ponteiros para todos os heróis do mundo, 
// o indice do vetor é o id dos heróis.
// Retorna o vetor de ponteiros para os heróis
struct heroi_t **iniciaHerois(int numHerois) {
  
  struct heroi_t **herois = malloc(numHerois * sizeof(struct heroi_t *));
  if (herois == NULL) {
    printf("Erro ao alocar\n");
    return NULL;
  }

  // Cria todos os heróis do mundo com id igual ao iterador do loop
  for (int i = 0; i < numHerois; i++) {
    herois[i] = criarHeroi(i);
  }

  return herois;
}

// Cria uma base dinâmicamente e incializa suas características.
// Retorna um ponteiro para essa base.
struct base_t *criarBase(unsigned int idB) {
  
  struct base_t *base = malloc(sizeof(struct base_t));
  if (base == NULL) {
    printf("Erro ao alocar\n");
    return NULL;
  }

  // Definição das caracterísitcas da base
  base->presentes = cjto_cria(N_HEROIS);
  if (base->presentes == NULL) {
    printf("Erro ao alocar\n");
    free(base);
    return NULL;
  }
  base->local = malloc(sizeof(struct coord_t));
  if (base->local == NULL) {
    printf("Erro ao alocar\n");
    cjto_destroi(base->presentes);
    free(base);
    return NULL;
  }
  base->espera = fila_cria();
  if (base->espera == NULL) {
    printf("Erro ao alocar\n");
    cjto_destroi(base->presentes);
    free(base->local);
    free(base);
    return NULL;
  }
  base->id = idB;
  base->local->x = aleat(0, N_TAMANHO_MUNDO - 1);
  base->local->y = aleat(0, N_TAMANHO_MUNDO - 1);
  base->lotacao = aleat(3, 10);
  base->tamEsperaMax = 0;
  base->missoes = 0;

  return base;
}

// Cria um vetor para armazenar as bases do mundo, 
// o indíce do vetor é o id da base.
// Retorna um vetor de ponteiros para as bases
struct base_t **iniciaBases(int numBases) {

  struct base_t **bases = malloc(numBases * sizeof(struct base_t *));
  if (bases == NULL) {
    printf("Erro ao alocar\n");
    return NULL;
  }

  // Cria todos as bases do mundo com id igual ao iterador do loop
  for (int i = 0; i < numBases; i++) {
    bases[i] = criarBase(i);
  }

  return bases;
}

// Cria uma missão dinâmicamente e incializa suas características.
// Retorna um ponteiro para essa missão
struct missao_t *criarMissao(unsigned int idM) {
  
  struct missao_t *mis = malloc(sizeof(struct missao_t));
  if (mis == NULL) {
    printf("Erro ao alocar\n");
    return NULL;
  }

  // Atribuição das caracteristicas da missão
  mis->habilidades = cjto_aleat(aleat(6, 10), N_HABILIDADES);
  if (mis->habilidades == NULL) {
    printf("Erro ao alocar\n");
    free(mis);
    return NULL;
  }
  mis->local = malloc(sizeof(struct coord_t));
  if (mis->local == NULL) {
    printf("Erro ao alocar\n");
    cjto_destroi(mis->habilidades);
    free(mis);
    return NULL;
  }
  mis->id = idM;
  mis->local->x = aleat(0, N_TAMANHO_MUNDO - 1);
  mis->local->y = aleat(0, N_TAMANHO_MUNDO - 1);
  mis->tentativas = 0;
  // define se a missão ja foi concluida(1) ou não(0)
  mis->feita = 0; 

  return mis;
}


// Cria um vetor para armazenar os ponteiros para as missoes do mundo, 
// o indíce do vetor é o id da respectiva missão.
// Retorna o vetor de ponteiros das missões.
struct missao_t **iniciaMissao(int numMissoes) {
  
  struct missao_t **missoes = malloc(numMissoes * sizeof(struct missao_t *));
  if (missoes == NULL) {
    printf("Erro ao alocar\n");
    return NULL;
  }

  // Cria todos as missões do mundo com id igual ao iterador do loop
  for (int i = 0; i < numMissoes; i++) {
    missoes[i] = criarMissao(i);
  }

  return missoes;
}

// Cria dinâmicamente a estrutura do mundo e inicializa suas características
// Retorna um ponteiro para esse mundo.
struct mundo_t *criarMundo() {
  
  struct mundo_t *mundo = malloc(sizeof(struct mundo_t));
  if (mundo == NULL) {
    printf("Erro ao alocar\n");
    return NULL;
  }

  // Inicialização das características do mundo
  
  mundo->TamMundo = malloc(sizeof(struct coord_t));
  if (mundo->TamMundo == NULL) {
    printf("Erro ao alocar TamMundo\n");
    free(mundo);
    return NULL;
  }

  mundo->TamMundo->x = N_TAMANHO_MUNDO;
  mundo->TamMundo->y = N_TAMANHO_MUNDO;
  mundo->CompostosV = N_COMPOSTOS_V;
  mundo->NHerois = N_HEROIS;
  mundo->herois = iniciaHerois(N_HEROIS);
  if (mundo->herois == NULL) {
    printf("Erro ao iniciar os heróis\n");
    return NULL;
  }
  mundo->NBases = N_BASES;
  mundo->bases = iniciaBases(N_BASES);
  if(mundo->bases == NULL) {
    printf("Erro ao alocar vetor de bases\n");
    return NULL;
  }
  mundo->NMissoes = N_MISSOES;
  mundo->missoes = iniciaMissao(N_MISSOES);
  if(mundo->missoes == NULL) {
    printf("Erro ao alocar vetor de missoes\n");
    return NULL;
  }
  mundo->NHabilidades = N_HABILIDADES;
  mundo->Relogio = 0;

  return mundo;
}


// Libera as alocações de memória dos vetores dos herois,
// das bases, das missões e as estruturas do mundo.
// Retorna um ponteiro nulo após destruir com sucesso.
struct mundo_t *destroiMundo(struct mundo_t *mundo) {
  if (mundo == NULL) {
    printf("Ponteiro do mundo é nulo\n");
    return NULL;
  }

  // Destroi herois
  for (int i = 0; i < N_HEROIS; i++) {
    cjto_destroi(mundo->herois[i]->habilidades);
    free(mundo->herois[i]);
  }
  free(mundo->herois);

  // Destroi bases
  for(int i = 0; i < N_BASES; i++) {
    cjto_destroi(mundo->bases[i]->presentes);
    fila_destroi(mundo->bases[i]->espera);
    free(mundo->bases[i]->local);
    free(mundo->bases[i]);
  }
  free(mundo->bases);

  //Destroi missoes
  for(int i = 0; i < N_MISSOES; i++) {
    cjto_destroi(mundo->missoes[i]->habilidades);
    free(mundo->missoes[i]->local);
    free(mundo->missoes[i]);
  }
  free(mundo->missoes);

  // Destroi os outros elementos alocados dinâmicamento do mundo
  free(mundo->TamMundo); 

  // Libera o ponteiro para o mundo
  free(mundo);
  return NULL;
}

// Cria um evento para armazenar os dados a serem adicionados na Fila de Prioridades
// Retorna um ponteiro para o evento.
struct evento_t *criaEvento(int tempo, int tipo, int primDado, int segDado) {
  
  struct evento_t *evento = malloc(sizeof(struct evento_t));
  if (evento == NULL) {
    printf("Erro ao alocar\n");
    return NULL;
  }

  // Atribuição dos valores às características do evento
  evento->tempo = tempo;
  evento->tipo = tipo;
   // id do heroi ou da missao
  evento->primDado = primDado;
  // id da base
  evento->segDado = segDado; 

  return evento;
}

// Destroi um evento passado por parâmetro.
// Retorna o tipo do evento.
int destroiEvento(struct evento_t *evento) {
  if (evento == NULL) {
    printf("Ponteiros nulos\n");
  }

  int tipo = evento->tipo;
  free(evento);

  return tipo;
}

// Calcula a distância cartesiana entre duas bases.
// Retorna a distância ou -1 em caso de erro.
int disCartBases(struct heroi_t *heroi, struct mundo_t *m, struct base_t *base) {
  if (m == NULL || heroi == NULL || base == NULL) {
    printf("Ponteiros nulos\n");
    return -1;
  }

  int difX = m->bases[heroi->base]->local->x - base->local->x;    
  int difY = m->bases[heroi->base]->local->y - base->local->y;
  int distancia = sqrt((difX * difX) + (difY * difY));

  return distancia;
}

// Calcula a distância entre uma base e o local de uma missão.
// Retorna a distância ou -1 em caso de erro.
int disCartMissao(struct mundo_t *m, struct missao_t *mis, int i) {
  if (m == NULL || mis == NULL) {
    printf("Ponteiros nulos\n");
    return -1;
  }

  int difX = mis->local->x - m->bases[i]->local->x;
  int difY = mis->local->y - m->bases[i]->local->y;
  int distancia = sqrt((difX * difX) + (difY * difY));

  return distancia;
}


// Atualiza a base do herói e define se ESPERA na base ou se DESISTE.
// Adiciona o evento definido na Fila de Eventos Futuros.
void chega(struct fprio_t *lef, unsigned int tempo, struct heroi_t *heroi, struct base_t *base) {
  if (lef == NULL || heroi == NULL || base == NULL) {
    printf("Ponteiros nulos\n");
    return;
  }

  // Verifica se o herói está vivo
  if(!(heroi->estado)) {
    printf("HEROI MORTO\n");
    return;
  }
    
  // Atualiza base do heroi
  heroi->base = base->id;
  
  int espera = 0;
  // Se houver vagas na base e se a fila estiver vazia, heróis espera.
  if ((cjto_card(base->presentes) < base->lotacao) && !(fila_tamanho(base->espera))) {
    espera = 1;
  // Se a paciência do herói permitir, ele espera.
  } else if ((heroi->paciencia) > (10 * fila_tamanho(base->espera))) {
    espera = 1;
  }
  
  if (espera) {
    struct evento_t *evento = criaEvento(tempo, ESPERA, heroi->id, base->id);
    if (evento == NULL) {
      printf("Erro ao alocar\n");
      return;
    }

    if (fprio_insere(lef, evento, ESPERA, tempo) < 0) {
      printf("Erro ao alocar ESPERA na LEF\n");
      return;
    }

    printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA\n", tempo, heroi->id, base->id, cjto_card(base->presentes), base->lotacao);
    return;
  // Caso o herói não for esperar.
  } else {
    // Insere na LEF o evento DESISTIR
    struct evento_t *evento = criaEvento(tempo, DESISTE, heroi->id, base->id);
    if (evento == NULL) {
      printf("Erro ao alocar\n");
      return;
    }

    if (fprio_insere(lef, evento, DESISTE, tempo) < 0) {
      printf("Erro ao alocar DESISTE na LEF\n");
      return;
    }
  
    printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE\n", tempo, heroi->id, base->id, cjto_card(base->presentes), base->lotacao);
  }
}

// O herói entra na fila de espera da base e o porteiro é avisado.
// Adiciona o evento AVISA na Lista de Eventos Futuros.
void espera(struct fprio_t *lef, unsigned int tempo, struct heroi_t *heroi, struct base_t *base) {
  if (lef == NULL || heroi == NULL || base == NULL) {
    printf("Ponteiros nulos\n");
    return;
  }

  // Ignora eventos de heróis mortos.
  if(!(heroi->estado)) {
    printf("HEROI MORTO\n");
    return;
  }

  printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", tempo, heroi->id, base->id, fila_tamanho(base->espera));

  // Insere na lista de espera da base o id do herói.
  if (fila_insere(base->espera, heroi) < 0) {
    printf("Erro ao inserir heroi na fila de espera da base\n");
    return;
  }

  // Encontra o tamanho máximo que a fila de espera da base ja teve.
  if (base->tamEsperaMax < fila_tamanho(base->espera)) {
    base->tamEsperaMax = fila_tamanho(base->espera);
  }

  // Cria o evento de avisar o porteiro e o adiciona na lef
  struct evento_t *evento = criaEvento(tempo, AVISA, heroi->id, base->id);
  if (evento == NULL) {
    printf("Erro ao alocar\n");
    return;
  }

  if (fprio_insere (lef, evento, AVISA, tempo) < 0) {
    printf("Erro ao inserir evento AVISA na LEF\n");
    return;
  }
}

// Escolhe uma base aleatória para o herói viajar
// Aloca o evento viajar na LEF.
void desiste(struct fprio_t *lef, unsigned int tempo, struct heroi_t *heroi, struct base_t *base) { 
  if (lef == NULL || heroi == NULL || base == NULL) {
    printf("Ponteiros nulos\n");
    return;
  }

  // Ignora eventos de heróis mortos.
  if(!heroi->estado) {
    printf("HEROI MORTO\n");
    return;
  }
  
  printf("%6d: DESIST HEROI %2d BASE %d\n", tempo, heroi->id, base->id);

  // define um id de base aleatória
  int id = aleat(0, N_BASES - 1);
  
  // Cria o evento de viajar e o adiciona na lef
  struct evento_t *evento = criaEvento(tempo, VIAJA, heroi->id, id);
  if (evento == NULL) {
    printf("Erro ao alocar\n");
    return;
  }
  if (fprio_insere(lef, evento, VIAJA, tempo) < 0) {
    printf("Erro ao inserir eveto VIAJA na LEF\n");
    return;
  }
}

// O porteiro da base trata a fila de espera, 
// agendando a entrada de heróis na base até não ser mais possível.
// Aloca eventos ENTRA na LEF.
void avisa(struct fprio_t *lef, unsigned int tempo, struct base_t *base) {
  if (base == NULL || lef == NULL) {
    printf("Ponteiros nulos\n");
    return;
  }

  printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA ", tempo, base->id, cjto_card(base->presentes), base->lotacao);
  fila_imprime(base->espera);
  printf("\n");

  // Enquanto houver vaga na base e houver heróis esperando na fila, 
  // agenda a entrada deles.
  // Coloca o primeiro heroi da fila de espera para dentro da base e o retira da fila de espera
  while ((base->lotacao > cjto_card(base->presentes)) && (fila_tamanho(base->espera) > 0)) {
    
    struct heroi_t *h = (struct heroi_t *)base->espera->prim->item;
    
    // Verifica se o herói está morto, se estiver, não o adiciona na base.
    if (!h->estado) {
      fila_retira(base->espera); 
      continue;
    }

    printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", tempo, base->id, h->id);

    struct evento_t *evento = criaEvento(tempo, ENTRA, h->id, base->id);
    if (evento == NULL) {
      printf("Erro ao alocar\n");
      return;
    }

    if (fprio_insere(lef, evento, ENTRA, tempo) < 0) {
      printf("Erro ao inserir evento ENTRA na LEF\n");
      return;
    }

    fila_retira(base->espera);
  }
}

// Herói entra na base e define o tempo de permanência.
// Agenda na LEF a saida do herói da base.
void entra(struct fprio_t *lef, unsigned int tempo, struct heroi_t *heroi, struct base_t *base) {
  
  if (lef == NULL || base == NULL || heroi == NULL) {
    printf("Ponteiros nulos\n");
    return;
  }
  
  // Ignora eventos de heróis mortos.
  if(!heroi->estado) {
    printf("HEROI MORTO\n");
    return;
  }

  // Insere o herói no conjunto de presentes da base.
  cjto_insere(base->presentes, heroi->id);
  // Calcula o tempo de permanência na base.
  int tempPermBase = 15 + heroi->paciencia * aleat(1, 20);

  printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", tempo, heroi->id, base->id, cjto_card(base->presentes), base->lotacao, tempo + tempPermBase);
  
  // Cria o evento da saida do heroi no tempo definido.
  struct evento_t *evento = criaEvento(tempo + tempPermBase, SAI, heroi->id, base->id);
  if (evento == NULL) {
    printf("Erro ao alocar\n");
    return;
  }

  if (fprio_insere(lef, evento, SAI, tempo + tempPermBase) < 0) {
    printf("Erro ao inserir evento SAI na LEF\n");
    return;
  }
}

// Retira o herói do conjunto de presentes da base, agenda a viagem para outra base e avisa o porteiro
void sai(struct fprio_t *lef, unsigned int tempo, struct heroi_t *heroi, struct base_t *base) {
  if (lef == NULL || heroi == NULL || base == NULL) {
    printf("Ponteiro nulo\n");
    return;
  }

  // Remove o heroi do conjunto de presentes da base
  cjto_retira(base->presentes, heroi->id);

  // Define um id de base aleatória para viajar.
  int idBase = aleat(0, N_BASES - 1);

  printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", tempo, heroi->id, base->id, cjto_card(base->presentes), base->lotacao);

  // Cria e insere na LEF o evento VIAJAR
  struct evento_t *evento = criaEvento(tempo, VIAJA, heroi->id, idBase);
  if (evento == NULL) {
    printf("Erro ao alocar\n");
    return;
  }

  if (fprio_insere(lef, evento, VIAJA, tempo) < 0) {
    printf("Erro ao inserir evento VIAJA na LEF\n");
    return;
  }

  // Cria e insere na LEF o evento AVISAR
  struct evento_t *evento1 = criaEvento(tempo, AVISA, heroi->id, base->id);
  if (evento1 == NULL) {
    printf("Erro ao alocar\n");
    return;
  }

  if (fprio_insere(lef, evento1, AVISA, tempo) < 0) {
    printf("Erro ao inserir evento AVISA na LEF\n");
    return;
  }
}

// Define o tempo de deslocamento do herói de uma base para outra (pode ser para a mesma).
// Agenda na LEF o evento CHEGA de acordo com o tempo de viagem.
void viaja(struct fprio_t *lef, unsigned int tempo, struct heroi_t *heroi, struct base_t *baseD, struct mundo_t *mundo) {
  if (lef == NULL || heroi == NULL || baseD == NULL || mundo == NULL) {
    printf("Ponteiro nulo\n");
    return;
  }

  // Ignora eventos de heróis mortos
  if(!heroi->estado) {
    printf("HEROI MORTO\n");
    return;
  }

  // Calcula a distancia da viagem.
  int distancia = disCartBases(heroi, mundo, baseD);

  // Calcula a duração da viagem
  if(heroi->velocidade <= 0)
    return;
  int duracao = distancia / heroi->velocidade;

  printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", tempo, heroi->id, heroi->base, baseD->id, distancia, heroi->velocidade, tempo + duracao);

  // Cria e insere na LEF o evento CHEGAR
  struct evento_t *evento = criaEvento(tempo + duracao, CHEGA, heroi->id, baseD->id);
  if (evento == NULL) {
    printf("Erro ao alocar\n");
    return;
  }  
  if (fprio_insere(lef, evento, CHEGA, tempo + duracao) < 0) {
    printf("Erro ao inserir o evento CHEGA na LEF\n");
    return;
  }
}

// Herói morre e é retirado do conjunto de presentes da base que estava
// Agenda o evento AVISA na LEF.
void morre(struct fprio_t *lef, unsigned int tempo, struct heroi_t *heroi, struct missao_t *m, struct mundo_t *mundo) {
  if (lef == NULL || heroi == NULL || m == NULL || mundo == NULL) {
    printf("Ponteiro nulo\n");
    return;
  }
  
  // Retira o heroi da fila de presentes da base
  cjto_retira(mundo->bases[heroi->base]->presentes, heroi->id);

  // Define o herói como morto
  heroi->estado = 0;

  printf("%6d: MORRE HEROI %2d MISSAO %d\n", tempo, heroi->id, m->id);
  
  // Cria e insere na LEF o evento AVISAR o porteiro da base que o herói estava
  struct evento_t *evento = criaEvento(tempo, AVISA, heroi->id, heroi->base);
  if (evento == NULL) {
    printf("Erro ao alocar\n");
    return;
  }
  if (fprio_insere(lef, evento, AVISA, tempo) < 0) {
    printf("Erro ao inserir o evento AVISA na LEF\n");
    return;
  }
}

//  Realiza a missão de acordo com a proximidade e existência de uma base com heróis que tenham
// as habilidades requeridas. Se não existir, verifica a possibilidade de utilizar Composto V. Se não
// for possível, missão é impossível e é reagendada.
void missao(struct fprio_t *lef, unsigned int tempo, struct missao_t *m, struct mundo_t *mundo) {
  if (lef == NULL || m == NULL || mundo == NULL) {
    printf("Ponteiro nulo\n");
    return;
  }

  // Se a missão já foi concluída, prossegue.
  if(m->feita) {
    return;
  }

  m->tentativas++; 
  
  printf("%6d: MISSAO %d TENT %d HAB REQ: ", tempo, m->id, m->tentativas);
  cjto_imprime(m->habilidades);
  printf("\n");
  

  // Vetor para as distâncias e identificação das bases.
  struct dist_t *d = malloc(N_BASES * sizeof(struct dist_t));
  if (d == NULL) {
    printf("Erro ao alocar memória");
    return;
  }

  // Calcula as distâncias da missão para cada base que existe no mundo
  for (size_t i = 0; i < N_BASES; i++) {
    d[i].distancia = disCartMissao(mundo, m, i);
    d[i].idBase = mundo->bases[i]->id;
    if (d[i].distancia < 0) {
      return;
    }
  }

  // Ordenar as distâncias de forma crescente utilizando o bubble sort
  for (size_t i = 0; i < N_BASES-1; i++) {
    for (size_t j = 0; j < N_BASES-i-1; j++) {
      if (d[j].distancia > d[j+1].distancia) {
        struct dist_t temp = d[j];
        d[j] = d[j+1];
        d[j+1] = temp;
      }
    }
  }

  // Verifica se há presentes na base
  for (size_t i = 0; i < N_BASES; i++) {
    if (!cjto_card(mundo->bases[d[i].idBase]->presentes)) {
      continue;
    }

    // Armazena a união das habilidades de todos os heróis da base
    struct cjto_t *habilidadesBase;
    habilidadesBase = cjto_cria(N_HABILIDADES);
    if (habilidadesBase == NULL) {
      printf("Erro ao alocar\n");
      free(d);
      return;
    }

    // Passa por todos os herois do mundo e verifica se ele pertence à base dessa iteração
    // se sim, adiciona suas habilidades ao conjunto da base.
    for(size_t j = 0; j < N_HEROIS; j++) {
      if(cjto_pertence(mundo->bases[d[i].idBase]->presentes, mundo->herois[j]->id) && (mundo->herois[j]->estado)) {
        struct cjto_t *temp = cjto_uniao(habilidadesBase, mundo->herois[j]->habilidades);
        cjto_destroi(habilidadesBase);
        habilidadesBase = temp;
      }
    }

    // Verifica se as habilidades da base estão contidas nas habilidades requeridas pela missao
    if(cjto_contem(habilidadesBase, m->habilidades)) {
      // Altera o status de realização da missão
      m->feita = 1;

      // Busca os herois que pertencem à base e aumenta a experiência
      for(size_t k = 0; k < N_HEROIS; k++) {
        if(cjto_pertence(mundo->bases[d[i].idBase]->presentes, mundo->herois[k]->id)) {
            (mundo->herois[k]->xp)++;
        }
      }
      
      // Aumenta a quantidade de missoes feitas pela base
      (mundo->bases[d[i].idBase]->missoes)++;
      
      printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: ", tempo, m->id, d[i].idBase);
      cjto_imprime(habilidadesBase);
      printf("\n");

      cjto_destroi(habilidadesBase);
      free(d);
      return;
    }
    cjto_destroi(habilidadesBase);    
  } 

  // Caso nenhuma base possua as habilidades necessárias,
  // verifica se é posível usar CompostosV.
  if ((mundo->CompostosV > 0) && ((tempo % 2500) == 0) && cjto_card(mundo->bases[d[0].idBase]->presentes)) {
    
    // Decrementa a quantidade de Compostos V e marca a missão como feita
    mundo->CompostosV--;
    m->feita = 1;

    // Procura o primeiro herói que pertence à base mais próxima
    int maior = -1;
    int i = 0;
    while ((maior == -1) && (i < N_HEROIS)) {
      if (cjto_pertence(mundo->bases[d[0].idBase]->presentes, mundo->herois[i]->id) && mundo->herois[i]->estado) {
        maior = mundo->herois[i]->id; // Salva o id do herói
      }
      i++;
    }

    // Existem heróis vivos na base
    if(maior != -1) {
      // Procura na base o herói com maior xp
      for (size_t i = 0; i < N_HEROIS; i++) {
        if (cjto_pertence(mundo->bases[d[0].idBase]->presentes, mundo->herois[i]->id)) {
          if (mundo->herois[i]->xp > mundo->herois[maior]->xp) {
            maior = mundo->herois[i]->id;
          }
        }
      }

      // Insere na LEF o evento de MORRE para o herói que usou o Composto V
      struct evento_t *evento = criaEvento(tempo, MORRE, maior, m->id);
      if (evento == NULL) {
        printf("Erro ao alocar\n");
        return;
      }
      if (fprio_insere(lef, evento, MORRE, tempo) < 0) {
        printf("Erro ao inserir o evento MORRE na LEF\n");
        return;
      }
  
      // Aumenta a experiência dos demais heróis da base
      for (size_t i = 0; i < N_HEROIS; i++) {
        if (cjto_pertence(mundo->bases[d[0].idBase]->presentes, mundo->herois[i]->id) && (mundo->herois[i]->estado)) {
          mundo->herois[i]->xp++;
        }
      }
      free(d);
      return;
    }
  }
  
  // Missão não é possível de ser realizada neste momento.
  printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo, m->id);
  
  // Reagenda a missão na LEF
  struct evento_t *evento = criaEvento(tempo + 24*60, MISSAO, m->id, -1);
  if (evento == NULL) {
      printf("Erro ao alocar\n");
      return;
  }
  if (fprio_insere(lef, evento, MISSAO, tempo + 24*60) < 0) {
    printf("Erro ao inserir evento MISSÃO na LEF\n");
    return;
  }
  free(d);
}

// Encerra a simulação e imprime o relatório.
void fim(unsigned int tempo, struct fprio_t *lef, struct mundo_t *m, int contadorEventos) {
  if (lef == NULL || m == NULL) {
    printf("Ponteiro nulo\n");
    return;
  }
  
  printf("%6d: FIM\n", tempo);

  // Imprime as informações dos heróis
  for (size_t i = 0; i < N_HEROIS; i++) {
    printf("HEROI %2d ", m->herois[i]->id);
    if(m->herois[i]->estado)
      printf("VIVO ");
    else
      printf("MORTO ");
    printf("PAC %3d ", m->herois[i]->paciencia);
    printf("VEL %3d ", m->herois[i]->velocidade);
    printf("EXP %4d ", m->herois[i]->xp);
    printf("HABS ");
    cjto_imprime(m->herois[i]->habilidades);
    printf("\n");
  }

  // Imprime as informações das bases
  for(size_t i = 0; i < N_BASES; i++) {
    printf("BASE %2d ", m->bases[i]->id);
    printf("LOT %2d ", m->bases[i]->lotacao);
    printf("FILA MAX %2d ", m->bases[i]->tamEsperaMax);
    printf("MISSOES %d\n", m->bases[i]->missoes);
  }

  printf("EVENTOS TRATADOS: %d\n", contadorEventos);

  int contMissoes = 0;
  // Conta quantas missões foram concluídas.
  for(size_t i = 0; i < N_MISSOES; i++) {
    if(m->missoes[i]->feita)
      contMissoes++;
  }

  // Calcula porcentagem de missões feitas
  float missoesFeitas = (float)(contMissoes) / N_MISSOES * 100;
  printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", contMissoes, N_MISSOES, missoesFeitas);
  
  // Calcula a média das tentativas de realizar uma missão
  int somaMis = 0;
  for (size_t i = 0; i < N_MISSOES; i++) {
    somaMis += m->missoes[i]->tentativas;
  }
  float media = (float)somaMis / N_MISSOES;

  // Calcula o mínimo de tentativas feitas
  int minimo = m->missoes[0]->tentativas;
  for (size_t i = 1; i < N_MISSOES; i++) {
    if (m->missoes[i]->tentativas < minimo) {
      minimo = m->missoes[i]->tentativas;
    }
  }

  // Calcula o máximo de tentativas feitas
  int maximo = m->missoes[0]->tentativas;
  for (size_t i = 1; i < N_MISSOES; i++) {
    if (m->missoes[i]->tentativas > maximo) {
      maximo = m->missoes[i]->tentativas;
    }
  }

  printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", minimo, maximo, media);

  // Contar herois mortos.
  int contHeroisM = 0;
  for(size_t i = 0; i < N_HEROIS; i++) {
    if(!(m->herois[i]->estado))
      contHeroisM++;
  }
  float txMorte = (float)contHeroisM / N_HEROIS * 100;
  printf("TAXA MORTALIDADE: %.1f%%\n", txMorte);

  printf("FIM SIMULAÇÃO.\n");
}