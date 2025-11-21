#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define o tamanho máximo da fila de peças
#define TAM_FILA 5
#define TAM_PILHA 3

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

// Estrutura da pilha linear para reserva de peças
typedef struct {
    Peca itens[TAM_PILHA]; // Array de peças reservadas
    int topo;              // Índice do topo da pilha
} Pilha;

// Inicializa a fila, zerando índices e tamanho
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
}

// Inicializa a pilha, topo = -1 (vazia)
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->tamanho == TAM_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
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

// Insere uma peça no topo da pilha (push)
// Retorna 1 se sucesso, 0 se pilha cheia
int push(Pilha *p, Peca pc) {
    if (pilhaCheia(p)) return 0;
    p->topo++;
    p->itens[p->topo] = pc;
    return 1;
}

// Remove a peça do topo da pilha (pop)
// Retorna 1 se sucesso, 0 se pilha vazia
int pop(Pilha *p, Peca *pc) {
    if (pilhaVazia(p)) return 0;
    *pc = p->itens[p->topo];
    p->topo--;
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

// Exibe o estado atual da pilha de reserva
void mostrarPilha(Pilha *p) {
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("(vazia)");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].tipo, p->itens[i].id);
        }
    }
    printf("\n");
}

// Troca a peça da frente da fila com o topo da pilha
int trocarFrenteFilaComTopoPilha(Fila *fila, Pilha *pilha) {
    if (filaVazia(fila) || pilhaVazia(pilha)) {
        return 0; // Não é possível trocar
    }
    int idxFila = fila->inicio;
    int idxPilha = pilha->topo;
    Peca temp = fila->itens[idxFila];
    fila->itens[idxFila] = pilha->itens[idxPilha];
    pilha->itens[idxPilha] = temp;
    return 1;
}

// Troca os 3 primeiros da fila com os 3 da pilha
int trocarTresFilaTresPilha(Fila *fila, Pilha *pilha) {
    if (fila->tamanho < 3 || pilha->topo != 2) {
        return 0; // Não é possível trocar
    }
    for (int i = 0; i < 3; i++) {
        int idxFila = (fila->inicio + i) % TAM_FILA;
        int idxPilha = pilha->topo - i;
        Peca temp = fila->itens[idxFila];
        fila->itens[idxFila] = pilha->itens[idxPilha];
        pilha->itens[idxPilha] = temp;
    }
    return 1;
}

int main() {
    Fila fila;      // Fila de peças futuras
    Pilha pilha;    // Pilha de peças reservadas
    int idSeq = 0;  // Sequencial para id das peças
    int opcao;      // Opção do menu
    Peca p;         // Peça auxiliar para remoção

    srand((unsigned)time(NULL)); // Inicializa gerador de números aleatórios
    inicializarFila(&fila);      // Inicializa a fila
    inicializarPilha(&pilha);    // Inicializa a pilha

    // Preenche a fila com 5 peças iniciais
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca(idSeq++));
    }

    // Loop principal do menu
    do {
        printf("\nEstado atual:\n");
        mostrarFila(&fila);   // Exibe a fila atual
        mostrarPilha(&pilha); // Exibe a pilha atual

        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");
        printf("Opção escolhida: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peça (remover da frente da fila)
                if (dequeue(&fila, &p)) {
                    printf("Peça [%c %d] jogada!\n", p.tipo, p.id);
                    enqueue(&fila, gerarPeca(idSeq++)); // Mantém a fila cheia
                } else {
                    printf("Fila vazia! Não há peça para jogar.\n");
                }
                break;
            case 2: // Reservar peça (move da fila para pilha)
                if (filaVazia(&fila)) {
                    printf("Fila vazia! Não há peça para reservar.\n");
                } else if (pilhaCheia(&pilha)) {
                    printf("Pilha cheia! Não é possível reservar mais peças.\n");
                } else {
                    dequeue(&fila, &p);
                    push(&pilha, p);
                    printf("Peça [%c %d] reservada!\n", p.tipo, p.id);
                    enqueue(&fila, gerarPeca(idSeq++)); // Mantém a fila cheia
                }
                break;
            case 3: // Usar peça reservada (remove do topo da pilha)
                if (pop(&pilha, &p)) {
                    printf("Peça reservada [%c %d] usada!\n", p.tipo, p.id);
                } else {
                    printf("Pilha vazia! Não há peça reservada para usar.\n");
                }
                break;
            case 4: // Trocar peça da frente da fila com o topo da pilha
                if (trocarFrenteFilaComTopoPilha(&fila, &pilha)) {
                    printf("Troca realizada entre a frente da fila e o topo da pilha.\n");
                } else {
                    printf("Não é possível realizar a troca (verifique se há peças suficientes).\n");
                }
                break;
            case 5: // Trocar os 3 primeiros da fila com as 3 peças da pilha
                if (trocarTresFilaTresPilha(&fila, &pilha)) {
                    printf("Troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
                } else {
                    printf("Não é possível realizar a troca (são necessárias 3 peças em cada estrutura).\n");
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

