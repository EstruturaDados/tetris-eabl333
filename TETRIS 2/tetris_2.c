#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5   // tamanho fixo da fila de peças futuras
#define TAM_PILHA 3  // tamanho máximo da pilha de reserva

// Estrutura que representa uma peça
typedef struct {
    char nome; // tipo ('I', 'O', 'T', 'L')
    int id;    // identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca pecas[TAM_FILA];
    int frente;
    int tras;
    int tamanho;
} Fila;

// Estrutura da pilha linear
typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

// Função que gera uma peça aleatória
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Inicializa a fila
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

// Inicializa a pilha
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

// Verifica se a fila está cheia
int filaCheia(Fila *fila) {
    return fila->tamanho == TAM_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *fila) {
    return fila->tamanho == 0;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *pilha) {
    return pilha->topo == TAM_PILHA - 1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

// Adiciona uma peça ao final da fila (enqueue)
void enfileirar(Fila *fila, Peca p) {
    if (filaCheia(fila)) return;
    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->pecas[fila->tras] = p;
    fila->tamanho++;
}

// Remove a peça da frente da fila (dequeue)
Peca desenfileirar(Fila *fila) {
    Peca removida = {'-', -1};
    if (filaVazia(fila)) return removida;
    removida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->tamanho--;
    return removida;
}

// Empilha uma peça na pilha de reserva
void empilhar(Pilha *pilha, Peca p) {
    if (pilhaCheia(pilha)) {
        printf("⚠️ Pilha cheia! Não é possível reservar mais peças.\n");
        return;
    }
    pilha->topo++;
    pilha->pecas[pilha->topo] = p;
}

// Desempilha uma peça (usar peça reservada)
Peca desempilhar(Pilha *pilha) {
    Peca removida = {'-', -1};
    if (pilhaVazia(pilha)) {
        printf("⚠️ Pilha vazia! Nenhuma peça reservada disponível.\n");
        return removida;
    }
    removida = pilha->pecas[pilha->topo];
    pilha->topo--;
    return removida;
}

// Exibe a fila e a pilha
void exibirEstado(Fila *fila, Pilha *pilha) {
    printf("\n=========================================\n");
    printf(" Estado Atual do Jogo - Tetris Stack\n");
    printf("=========================================\n");

    printf("\nFila de Peças: ");
    if (filaVazia(fila)) {
        printf("(vazia)");
    } else {
        for (int i = 0; i < fila->tamanho; i++) {
            int pos = (fila->frente + i) % TAM_FILA;
            printf("[%c %d] ", fila->pecas[pos].nome, fila->pecas[pos].id);
        }
    }

    printf("\nPilha de Reserva (Topo -> Base): ");
    if (pilhaVazia(pilha)) {
        printf("(vazia)");
    } else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
        }
    }
    printf("\n=========================================\n");
}

int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int idPeca = 0;
    int opcao;

    // Preenche a fila inicial
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca(idPeca++));
    }

    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpções de Ação:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: { // Jogar peça
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("🕹️ Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    enfileirar(&fila, gerarPeca(idPeca++));
                } else {
                    printf("⚠️ Fila vazia!\n");
                }
                break;
            }

            case 2: { // Reservar peça
                if (pilhaCheia(&pilha)) {
                    printf("⚠️ Pilha de reserva cheia! Libere espaço antes.\n");
                } else if (!filaVazia(&fila)) {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("📦 Peça reservada: [%c %d]\n", reservada.nome, reservada.id);
                    enfileirar(&fila, gerarPeca(idPeca++));
                }
                break;
            }

            case 3: { // Usar peça reservada
                Peca usada = desempilhar(&pilha);
                if (usada.id != -1)
                    printf("🎯 Peça usada: [%c %d]\n", usada.nome, usada.id);
                break;
            }

            case 0:
                printf("👋 Encerrando o jogo... até a próxima!\n");
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
