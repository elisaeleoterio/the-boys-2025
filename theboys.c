// Programa principal do projeto "The Boys - 2025/1"
// Autor: Elisa R. Eleotério, GRR 20241731

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "conjunto.h"
#include "fila.h"
#include "fprio.h"
#include "entidades.h"

// Definições dos valores para os eventos
#define CHEGA 1
#define ESPERA 2
#define DESISTE 3
#define AVISA 4
#define ENTRA 5
#define SAI 6
#define VIAJA 7
#define MORRE 8
#define MISSAO 9
#define FIM 10


// Programa principal para simular o mundo
int main () {
  
  srand(time(NULL));

  // Cria e inicializa todos os aspectos do mundo.
  struct mundo_t *mundo = criarMundo();
  if(mundo == NULL) {
    printf("Erro ao criar o mundo\n");
    return -1;
  }

  // Cria a Fila de Eventos Futuros utilizando uma fila de prioridades.
  struct fprio_t *lef = fprio_cria();
  if(lef == NULL) {
    printf("Erro ao criar a lista de eventos futuros\n");
    destroiMundo(mundo);
    return -1;
  }

  // EVENTOS INICIAIS:

  // Agenda a chegada dos heróis nas bases durante os 3 primeiros dias.
  for(int i = 0; i < N_HEROIS; i++) {
    int baseAleat = aleat(0, N_BASES - 1);
    int tempoChegada = aleat(0, 4320);
    
    struct evento_t *evento = criaEvento(tempoChegada, CHEGA, mundo->herois[i]->id, mundo->bases[baseAleat]->id);
    if(evento == NULL) {
      printf("Erro ao alocar evento\n");
      fprio_destroi(lef);
      destroiMundo(mundo);
      return -1;
    }
    if (fprio_insere(lef, evento, CHEGA, tempoChegada) < 0) { // como verificar se inseriu?
      printf("Erro ao alocar CHEGA na LEF\n");
      return -1;
    }
  }

  // Agenda a a realização das missões durante a simulação.
  for(int i = 0; i < N_MISSOES; i++) {
    int tempo = aleat(0, T_FIM_DO_MUNDO);
    struct evento_t *evento = criaEvento(tempo, MISSAO, mundo->missoes[i]->id, 0);
    if(evento == NULL) {
      printf("Erro ao alocar evento\n");
      fprio_destroi(lef);
      destroiMundo(mundo);
      return -1;
    }
    if (fprio_insere(lef, evento, MISSAO, tempo) < 0) {
      printf("Erro ao alocar MISSAO na LEF\n");
      return -1;
    };
  }


  int contEventos = 0;
  // Realiza os eventos até chegar no tempo final.
  while(mundo->Relogio <= T_FIM_DO_MUNDO) {
    struct evento_t *dado = (struct evento_t *)lef->prim->item;
    mundo->Relogio = dado->tempo;
    switch (lef->prim->tipo) {
      case CHEGA:
        chega(lef, mundo->Relogio, mundo->herois[dado->primDado], mundo->bases[dado->segDado]);
        break;
      case ESPERA:
        espera(lef, mundo->Relogio, mundo->herois[dado->primDado], mundo->bases[dado->segDado]);
        break;
      case DESISTE:
        desiste(lef, mundo->Relogio, mundo->herois[dado->primDado], mundo->bases[dado->segDado]);
        break;
      case AVISA:
        avisa(lef, mundo->Relogio, mundo->bases[dado->segDado]); 
        break;
      case ENTRA:
        entra(lef, mundo->Relogio, mundo->herois[dado->primDado], mundo->bases[dado->segDado]);
        break;
      case SAI:
        sai(lef, mundo->Relogio, mundo->herois[dado->primDado], mundo->bases[dado->segDado]);
        break;
      case VIAJA:
        viaja(lef, mundo->Relogio, mundo->herois[dado->primDado], mundo->bases[dado->segDado], mundo);
        break;
      case MORRE:
        morre(lef, mundo->Relogio, mundo->herois[dado->primDado], mundo->missoes[dado->segDado], mundo);
        break;
      case MISSAO:
        missao(lef, mundo->Relogio, mundo->missoes[dado->primDado], mundo);
        break;
      default:
        break;
    }
    contEventos++;

    destroiEvento(dado);
    int tipo, prio;
    fprio_retira(lef, &tipo, &prio);
  }

  // Ao fim da simulação
  fim(mundo->Relogio, lef, mundo, contEventos);

  destroiMundo(mundo);
  fprio_destroi(lef);

  return (0);
}

