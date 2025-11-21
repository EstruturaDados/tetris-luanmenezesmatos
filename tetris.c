#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define o tamanho máximo da fila de peças
#define TAM_FILA 5

// Struct que representa uma peça do Tetris
typedef struct {
    char tipo; // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;    // Identificador único da peça
} Peca;

// Estrutura da fila circular para armazenar as peças
typedef struct {
    Peca itens[TAM_FILA]; // Array de peças
    int inicio;           // Índice do início da fila
    int fim;              // Índice do final da fila
    int tamanho;          // Quantidade atual de peças na fila
} Fila;

// Inicializa a fila, zerando índices e tamanho
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->tamanho == TAM_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

// Gera uma peça aleatória com id único
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'}; // Tipos possíveis de peça
    int idx = rand() % 4;                // Seleciona tipo aleatório
    Peca p;
    p.tipo = tipos[idx];
    p.id = id;
    return p;
}

// Insere uma peça ao final da fila (enqueue)
// Retorna 1 se sucesso, 0 se fila cheia
int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return 0;
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA; // Avança circularmente
    f->tamanho++;
    return 1;
}

// Remove a peça da frente da fila (dequeue)
// Retorna 1 se sucesso, 0 se fila vazia
int dequeue(Fila *f, Peca *p) {
    if (filaVazia(f)) return 0;
    *p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA; // Avança circularmente
    f->tamanho--;
    return 1;
}

// Exibe o estado atual da fila de peças
void mostrarFila(Fila *f) {
    printf("\nFila de peças\n");
    int idx = f->inicio;
    for (int i = 0; i < f->tamanho; i++) {
        printf("[%c %d] ", f->itens[idx].tipo, f->itens[idx].id);
        idx = (idx + 1) % TAM_FILA;
    }
    printf("\n");
}

int main() {
    Fila fila;      // Fila de peças futuras
    int idSeq = 0;  // Sequencial para id das peças
    int opcao;      // Opção do menu
    Peca p;         // Peça auxiliar para remoção

    srand((unsigned)time(NULL)); // Inicializa gerador de números aleatórios
    inicializarFila(&fila);      // Inicializa a fila

    // Preenche a fila com 5 peças iniciais
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca(idSeq++));
    }

    // Loop principal do menu
    do {
        mostrarFila(&fila); // Exibe a fila atual
        printf("\nOpções de ação:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peça (remover da frente)
                if (dequeue(&fila, &p)) {
                    printf("Peça [%c %d] jogada!\n", p.tipo, p.id);
                } else {
                    printf("Fila vazia! Não há peça para jogar.\n");
                }
                break;
            case 2: // Inserir nova peça ao final
                if (filaCheia(&fila)) {
                    printf("Fila cheia! Não é possível inserir nova peça.\n");
                } else {
                    if (enqueue(&fila, gerarPeca(idSeq++))) {
                        printf("Nova peça inserida ao final da fila.\n");
                    }
                }
                break;
            case 0: // Sair do programa
                printf("Saindo...\n");
                break;
            default: // Opção inválida
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}

