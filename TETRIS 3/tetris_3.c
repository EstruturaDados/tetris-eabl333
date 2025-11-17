#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura que representa uma peça
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca pecas[TAM_FILA];
    int frente;
    int tras;
    int tamanho;
} Fila;

// Estrutura da pilha
typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

// ===== FUNÇÕES AUXILIARES =====

// Gera uma nova peça aleatória
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// ===== FILA =====

void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

int filaCheia(Fila *fila) {
    return fila->tamanho == TAM_FILA;
}

int filaVazia(Fila *fila) {
    return fila->tamanho == 0;
}

void enfileirar(Fila *fila, Peca p) {
    if (filaCheia(fila)) return;
    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->pecas[fila->tras] = p;
    fila->tamanho++;
}

Peca desenfileirar(Fila *fila) {
    Peca removida = {'-', -1};
    if (filaVazia(fila)) return removida;
    removida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->tamanho--;
    return removida;
}

// ===== PILHA =====

void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

int pilhaCheia(Pilha *pilha) {
    return pilha->topo == TAM_PILHA - 1;
}

int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

void empilhar(Pilha *pilha, Peca p) {
    if (pilhaCheia(pilha)) {
        printf("⚠️  Pilha cheia! Não é possível adicionar mais peças.\n");
        return;
    }
    pilha->topo++;
    pilha->pecas[pilha->topo] = p;
}

Peca desempilhar(Pilha *pilha) {
    Peca removida = {'-', -1};
    if (pilhaVazia(pilha)) {
        printf("⚠️  Pilha vazia! Nenhuma peça para usar.\n");
        return removida;
    }
    removida = pilha->pecas[pilha->topo];
    pilha->topo--;
    return removida;
}

// ===== EXIBIÇÃO =====

void exibirEstado(Fila *fila, Pilha *pilha) {
    printf("\n============================================\n");
    printf(" ESTADO ATUAL DO JOGO - TETRIS STACK\n");
    printf("============================================\n");

    printf("\nFila de peças: ");
    if (filaVazia(fila)) printf("(vazia)");
    else {
        for (int i = 0; i < fila->tamanho; i++) {
            int pos = (fila->frente + i) % TAM_FILA;
            printf("[%c %d] ", fila->pecas[pos].nome, fila->pecas[pos].id);
        }
    }

    printf("\nPilha de reserva (Topo -> Base): ");
    if (pilhaVazia(pilha)) printf("(vazia)");
    else {
        for (int i = pilha->topo; i >= 0; i--)
            printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
    }

    printf("\n============================================\n");
}

// ===== OPERAÇÕES DE TROCA =====

// Troca o topo da pilha com a frente da fila
void trocarTopoComFrente(Fila *fila, Pilha *pilha) {
    if (filaVazia(fila) || pilhaVazia(pilha)) {
        printf("⚠️  Não é possível trocar: fila ou pilha vazia.\n");
        return;
    }

    int frente = fila->frente;
    Peca temp = fila->pecas[frente];
    fila->pecas[frente] = pilha->pecas[pilha->topo];
    pilha->pecas[pilha->topo] = temp;

    printf("🔄 Troca realizada entre a peça da frente da fila e o topo da pilha!\n");
}

// Troca múltipla: 3 primeiras da fila com as 3 da pilha
void trocarTres(Fila *fila, Pilha *pilha) {
    if (fila->tamanho < 3 || pilha->topo < 2) {
        printf("⚠️  É necessário ter pelo menos 3 peças na fila e na pilha para a troca múltipla.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int posFila = (fila->frente + i) % TAM_FILA;
        Peca temp = fila->pecas[posFila];
        fila->pecas[posFila] = pilha->pecas[pilha->topo - i];
        pilha->pecas[pilha->topo - i] = temp;
    }

    printf("🔁 Troca múltipla realizada entre as 3 primeiras da fila e as 3 da pilha!\n");
}

// ===== PROGRAMA PRINCIPAL =====

int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int idPeca = 0;
    int opcao;

    // Inicializa fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca(idPeca++));
    }

    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça (remover da fila)\n");
        printf("2 - Reservar peça (mover da fila para a pilha)\n");
        printf("3 - Usar peça reservada (remover da pilha)\n");
        printf("4 - Trocar peça da fila com o topo da pilha\n");
        printf("5 - Trocar 3 primeiras da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: { // Jogar peça
                Peca jogada = desenfileirar(&fila);
                if (jogada.id != -1)
                    printf("🎮 Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                enfileirar(&fila, gerarPeca(idPeca++));
                break;
            }

            case 2: { // Reservar
                if (pilhaCheia(&pilha)) {
                    printf("⚠️  Pilha cheia! Libere espaço antes.\n");
                } else if (!filaVazia(&fila)) {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("📦 Peça reservada: [%c %d]\n", reservada.nome, reservada.id);
                    enfileirar(&fila, gerarPeca(idPeca++));
                }
                break;
            }

            case 3: { // Usar peça da pilha
                Peca usada = desempilhar(&pilha);
                if (usada.id != -1)
                    printf("🎯 Peça usada: [%c %d]\n", usada.nome, usada.id);
                break;
            }

            case 4: { // Troca individual
                trocarTopoComFrente(&fila, &pilha);
                break;
            }

            case 5: { // Troca múltipla
                trocarTres(&fila, &pilha);
                break;
            }

            case 0:
                printf("👋 Encerrando o jogo. Até a próxima!\n");
                break;

            default:
                printf("❌ Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
