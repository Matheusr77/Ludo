#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// estrutura dos peões
typedef struct {
    int id;
    int posTabx;
    int posTaby;
    char simbolo;
    char direc;
    int sentido;
    int cor;
} Peao;

Peao peaoJ1[4], peaoJ2[4];
char tabuleiro[9][9];

// protótipos
void setJogadores();
void definePeao(Peao *peao, int id, int x, int y, char simbolo, char direc, int sentido, int cor);
void mostrarTabuleiro();
void iniciarTabuleiro();
void andarHorizontal(Peao *peao);
void andarVertical(Peao *peao);
void moverPeao(Peao *peao, int passos, int cor);
void modoDebug();
int lancarDado();

int main() {
    int opcao;

    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL)); // inicializa a semente do rand

    do {
        printf("\033[1;36m=== MENU ===\033[0m\n");
        printf("1 - Jogar\n");
        printf("2 - Modo Debug\n");
        printf("3 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                iniciarTabuleiro();
                setJogadores();
                mostrarTabuleiro();
                break;
            case 2:
                modoDebug();
                break;
            case 3:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

    } while (opcao != 3);

    return 0;
}

void iniciarTabuleiro() {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            tabuleiro[i][j] = '-';
}

void mostrarTabuleiro() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++)
            printf("%c ", tabuleiro[i][j]);
        printf("\n");
    }
}

void definePeao(Peao *peao, int id, int x, int y, char simbolo, char direc, int sentido, int cor) {
    peao->id = id;
    peao->posTabx = x;
    peao->posTaby = y;
    peao->simbolo = simbolo;
    peao->direc = direc;
    peao->sentido = sentido;
    peao->cor = cor;

    tabuleiro[x][y] = simbolo;
}

void setJogadores() {
    definePeao(&peaoJ1[0], 0, 8, 1, 'T', 'v', -1, 1);
    definePeao(&peaoJ1[1], 0, 8, 2, 'C', 'v', -1, 1);
    definePeao(&peaoJ1[2], 0, 8, 3, 'A', 'v', -1, 1);
    definePeao(&peaoJ1[3], 0, 8, 4, 'R', 'v', -1, 1);

    definePeao(&peaoJ2[0], 0, 0, 4, 't', 'h', -1, 2);
    definePeao(&peaoJ2[1], 0, 1, 4, 'c', 'h', -1, 2);
    definePeao(&peaoJ2[2], 0, 2, 8, 'a', 'h', -1, 2); // Corrigido bug aqui
    definePeao(&peaoJ2[3], 0, 3, 8, 'r', 'h', -1, 2);
}

void andarVertical(Peao *peao) {
    peao->posTabx += peao->sentido;

    // Virar no canto superior esquerdo ou inferior esquerdo
    if (peao->posTabx == 0 && peao->posTaby == 4) {
        peao->direc = 'h';
        peao->sentido = 1;
    } else if (peao->posTabx == 8 && peao->posTaby == 4) {
        peao->direc = 'h';
        peao->sentido = -1;
    }
}

void andarHorizontal(Peao *peao) {
    peao->posTaby += peao->sentido;

    // Virar no canto superior direito ou inferior direito
    if (peao->posTaby == 8 && peao->posTabx == 0) {
        peao->direc = 'v';
        peao->sentido = 1;
    } else if (peao->posTaby == 0 && peao->posTabx == 8) {
        peao->direc = 'v';
        peao->sentido = -1;
    }
}

void moverPeao(Peao *peao, int passos, int cor) {
    tabuleiro[peao->posTabx][peao->posTaby] = '-';

    for (int i = 0; i < passos; i++) {
        if (peao->direc == 'v')
            andarVertical(peao);
        else
            andarHorizontal(peao);
    }

    tabuleiro[peao->posTabx][peao->posTaby] = peao->simbolo;
}

int lancarDado() {
    return rand() % 6 + 1; // valor entre 1 e 6
}

void modoDebug() {
    iniciarTabuleiro();

    Peao teste;
    definePeao(&teste, 0, 8, 1, 'T', 'v', -1, 0);

    printf("\nPosição inicial:\n");
    mostrarTabuleiro();

    int resultado = lancarDado();
    printf("\nLançando dado... Resultado: %d\n", resultado);
    moverPeao(&teste, resultado, 0);

    printf("\nApós movimento:\n");
    mostrarTabuleiro();
}
