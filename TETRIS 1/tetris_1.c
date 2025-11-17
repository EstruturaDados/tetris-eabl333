#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_MAX 5 // tamanho fixo da fila

// Estrutura que representa uma peça do jogo
typedef struct {
    char nome; // tipo da peça ('I', 'O', 'T', 'L')
    int id;    // identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca pecas[TAM_MAX];
    int frente;
    int tras;
    int tamanho;
} Fila;

// Função para inicializar a fila
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

// Função para verificar se a fila está cheia
int filaCheia(Fila *fila) {
    return fila->tamanho == TAM_MAX;
}

// Função para verificar se a fila está vazia
int filaVazia(Fila *fila) {
    return fila->tamanho == 0;
}

// Função que gera uma nova peça automaticamente
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Função para inserir uma peça (enqueue)
void enfileirar(Fila *fila, Peca peca) {
    if (filaCheia(fila)) {
        printf("⚠️ A fila está cheia! Não é possível adicionar nova peça.\n");
        return;
    }
    fila->tras = (fila->tras + 1) % TAM_MAX;
    fila->pecas[fila->tras] = peca;
    fila->tamanho++;
}

// Função para remover uma peça (dequeue)
void desenfileirar(Fila *fila) {
    if (filaVazia(fila)) {
        printf("⚠️ A fila está vazia! Nenhuma peça para jogar.\n");
        return;
    }
    Peca jogada = fila->pecas[fila->frente];
    printf("🕹️ Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
    fila->frente = (fila->frente + 1) % TAM_MAX;
    fila->tamanho--;
}

// Função para exibir o estado atual da fila
void exibirFila(Fila *fila) {
    printf("\n=== Fila de Peças ===\n");
    if (filaVazia(fila)) {
        printf("(vazia)\n");
        return;
    }

    int i, pos;
    for (i = 0; i < fila->tamanho; i++) {
        pos = (fila->frente + i) % TAM_MAX;
        printf("[%c %d] ", fila->pecas[pos].nome, fila->pecas[pos].id);
    }
    printf("\n=====================\n");
}

int main() {
    srand(time(NULL)); // semente aleatória para gerar peças

    Fila fila;
    inicializarFila(&fila);

    int idPeca = 0;
    int opcao;

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAM_MAX; i++) {
        enfileirar(&fila, gerarPeca(idPeca++));
    }

    do {
        exibirFila(&fila);
        printf("\nOpções:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                desenfileirar(&fila);
                break;
            case 2:
                enfileirar(&fila, gerarPeca(idPeca++));
                break;
            case 0:
                printf("Encerrando o jogo... 🎮\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
