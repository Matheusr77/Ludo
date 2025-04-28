#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Verifica o SO e importa a biblioteca correspondente
#ifdef __linux__
    #include <unistd.h>
#elif __WIN32
    #include <Windows.h>
#else

#endif

//Definido limite de casas
#define limite 39

//Declaração da struct peão
typedef struct peao{
    int pos;
    int posTabx;
    int posTaby;
    char simb;
    char direc;
    int sentido;
    int ID;
}Peao;

//Declaração dos protótipos das funções
void mostrarTabuleiro();
void mostraMenu();
void definePeao(Peao *peao, int pos, int coordx, int coordy, char simb, char direct, int ID, int sentido);
void moverPeao(Peao *peao, int quantCasas, int indice);
void iniciar();
void setJogadores();
int lancarDado();
void limparTela();
int jogadorIniciado(Peao peao[]);
void tirardeCasa(Peao *peao);
int estaEmCasa(Peao peao);
void hvDirect(Peao *peao);
int verificaQualPeao(Peao *peao);
void Jogar(int dado, Peao *peoes);

//declaração do tabuleiro
char tabuleiro[11][11] = {
    ' ',' ',' ',' ','o','o','o',' ',' ',' ',' ',
    ' ',' ',' ',' ','o','#','#',' ',' ',' ',' ',
    ' ',' ',' ',' ','o','#','o',' ',' ',' ',' ',
    ' ',' ',' ',' ','o','#','o',' ',' ',' ',' ',
    'o','#','o','o','o','#','o','o','o','o','o',
    'o','#','#','#','#',' ','#','#','#','#','o',
    'o','o','o','o','o','#','o','o','o','#','o',
    ' ',' ',' ',' ','o','#','o',' ',' ',' ',' ',
    ' ',' ',' ',' ','o','#','o',' ',' ',' ',' ',
    ' ',' ',' ',' ','#','#','o',' ',' ',' ',' ',
    ' ',' ',' ',' ','o','o','o',' ',' ',' ',' '
};

//declaração do array de peões
Peao peaoJ1[4];
Peao peaoJ2[4];
Peao peaoJ3[4];
Peao peaoJ4[4];

//declaração de outras  variáveis
int quantJogadores;
int playing = 0;
int turno = 0;

int main(){
    mostraMenu();
    return 0;
}

void limparTela(){    
    #ifdef __linux__
        system("clear");
    #elif __WIN32
        system("cls");
    #else

    #endif
}

void mostrarTabuleiro(){ 
    for(int i = 0; i < 11; i++){
        for(int j = 0; j < 11; j++){
            printf("%3c",tabuleiro[i][j]);
        }
        printf("\n");
    }
}

void mostraMenu(){
    limparTela();
    int opc;
    printf("\n\n\n");
    printf(".##.......##.....##.########...#######.\n");
    printf(".##.......##.....##.##.....##.##.....##\n");
    printf(".##.......##.....##.##.....##.##.....##\n");
    printf(".##.......##.....##.##.....##.##.....##\n");
    printf(".##.......##.....##.##.....##.##.....##\n");
    printf(".##.......##.....##.##.....##.##.....##\n");
    printf(".########..#######..########...#######.\n");
    printf("\n\n\n");


    printf("Escolha uma opção no menu: \n");
    printf("1 - Novo jogo\n");
    printf("0 - Sair\n");
    printf("Sua escolha: ");
    scanf("%d", &opc);
    switch (opc){
    case 1:
        /*printf("Quantidade de jogadores: ");
        scanf("%d", &quantJogadores);*/
        quantJogadores = 2;
        limparTela();
        iniciar();
        break;
    default:
        printf("Opção Inválida\n");
        break;
    }
}

void iniciar(){
    setJogadores();
    playing = 1;
    turno = 1;
    while (playing){
        printf("Turno do Jogador %d \n", turno);
        mostrarTabuleiro();
        int dado = lancarDado();
        printf("Resultado: %d \n", dado);
        switch (turno){
            case 1:
                Jogar(dado, peaoJ1);
                break;
            case 2:
                Jogar(dado, peaoJ2);
                break;
            default:
                break;
        }
        printf("Pressione Enter para continuar...\n");
        getchar();
        limparTela();
        if(turno == quantJogadores){
            turno = 1;
        }else{
            turno++;
        }
    }   
}

void setJogadores(){
    switch (quantJogadores){
    case 2:
        //Define posição inicial dos peões do jogador 1
        tabuleiro[8][1] = '1';
        definePeao(&peaoJ1[0], 0, 8, 1, '1', 'v', 0, -1);

        tabuleiro[8][2] = '1';
        definePeao(&peaoJ1[1], 0, 8, 2, '1', 'v', 1, -1);

        tabuleiro[9][1] = '1';
        definePeao(&peaoJ1[2], 0, 9, 1, '1', 'v', 2, -1);

        tabuleiro[9][2] = '1';
        definePeao(&peaoJ1[3], 0, 9, 2, '1', 'v', 3, -1);

        //Define posição inicial dos peões do jogador 2

        tabuleiro[1][8] = '2';
        definePeao(&peaoJ2[0], 0, 1, 8, '2', 'v', 0, 1);

        tabuleiro[1][9] = '2';
        definePeao(&peaoJ2[1], 0, 1, 9, '2', 'v', 1, 1);

        tabuleiro[2][8] = '2';
        definePeao(&peaoJ2[2], 0, 1, 8, '2', 'v', 2, 1);

        tabuleiro[2][9] = '2';
        definePeao(&peaoJ2[3], 0, 2, 9, '2', 'v', 3, 1);
        break;
    default:
        printf("Não é possível jogar com essa quantidade de jogadores :( \n");
        break;
    }
}

void moverPeao(Peao *peoes, int quantCasas, int indice){
    //Loop para mover o peão uma casa por vez
    for (int i = 0; i < quantCasas; i++){
        //Verifica se é necessário alterar o sentido de movimentação do peão
        hvDirect(&peoes[indice]);
        //Remove o peão da posição em que ele está no tabuleiro
        tabuleiro[peoes[indice].posTabx][peoes[indice].posTaby] = 'o';

        //debug,=================remover==============
        printf("\nPosição: %d %d\n", peoes[indice].posTabx, peoes[indice].posTaby);

        //verifica se o peão está na posição 0
        if(peoes[indice].pos == 0){
            //remove o peão da casa
            tabuleiro[peoes[indice].posTabx][peoes[indice].posTaby] = ' ';
            //coloca o peão na respectiva casa de seu time
            if(peoes[indice].simb == '1'){
                tabuleiro[9][4] = peoes[indice].simb;
                peoes[indice].posTabx = 9;
                peoes[indice].posTaby = 4;
            }else if(peoes[indice].simb == '2'){
                tabuleiro[1][6] = peoes[indice].simb;
                peoes[indice].posTabx = 1;
                peoes[indice].posTaby = 6;
            }else if(peoes[indice].simb == '3'){
                tabuleiro[6][9] = peoes[indice].simb;
                peoes[indice].posTabx = 6;
                peoes[indice].posTaby = 9;
            }else if(peoes[indice].simb == '4'){
                tabuleiro[9][4] = peoes[indice].simb;
                peoes[indice].posTabx = 9;
                peoes[indice].posTaby = 4;
            }
            peoes[indice].pos = 1;
        }else if(peoes[indice].direc == 'v'){
            peoes[indice].posTabx += peoes[indice].sentido;
        }else if(peoes[indice].direc == 'h'){
            peoes[indice].posTaby += peoes[indice].sentido;
        }else{
            printf("Posição indefinida");
        }
        tabuleiro[peoes[indice].posTabx][peoes[indice].posTaby] = peoes[indice].simb;
        //soma na posição interna do vetor
        peoes[indice].pos++;
        printf("\nPosição: %d %d %c\n", peoes[indice].posTabx, peoes[indice].posTaby, peoes[indice].simb);
        
    }    
}

void hvDirect(Peao *peao){
    if (peao->posTabx == 0 && peao->posTaby == 4){
        peao->direc = 'h';
        peao->sentido = 1;
    }else if(peao->posTabx == 0 && peao->posTaby == 6){
        peao->direc = 'v';
        peao->sentido = 1;
    }else if(peao->posTabx == 4 && peao->posTaby == 6){
        peao->direc = 'h';
        peao->sentido = 1;
    }else if(peao->posTabx == 4 && peao->posTaby == 10){
        peao->direc = 'v';
        peao->sentido = 1;
    }else if(peao->posTabx == 6 && peao->posTaby == 10){
        peao->direc = 'h';
        peao->sentido = -1;
    }else if(peao->posTabx == 6 && peao->posTaby == 6){
        peao->direc = 'v';
        peao->sentido = 1;
    }else if(peao->posTabx == 10 && peao->posTaby == 6){
        peao->direc = 'h';
        peao->sentido = -1;
    }else if(peao->posTabx == 10 && peao->posTaby == 4){
        peao->direc = 'v';
        peao->sentido = -1;
    }else if(peao->posTabx == 6 && peao->posTaby == 4){
        peao->direc = 'h';
        peao->sentido = -1;
    }else if(peao->posTabx == 6 && peao->posTaby == 0){
        peao->direc = 'v';
        peao->sentido = -1;
    }else if(peao->posTabx == 4 && peao->posTaby == 0){
        peao->direc = 'h';
        peao->sentido = 1;
    }else if(peao->posTabx == 4 && peao->posTaby == 4){
        peao->direc = 'v';
        peao->sentido = -1;
    }else{
        if (peao->posTabx == 9 && peao->posTaby == 5 && peao->simb == '1'){
            peao->direc = 'h';
            peao->sentido = -1;
        }else if(peao->posTabx == 0 && peao->posTaby == 5 && peao->simb == '2'){
            peao->direc = 'v';
            peao->sentido = 1;
        }else if(peao->posTabx == 5 && peao->posTaby == 5){
            printf("Jogador %c é o vencedor");
            playing = 0;
        }
    }
}

void definePeao(Peao *peao, int pos, int coordx, int coordy, char simb, char direct, int ID, int sentido){
    peao->pos = pos;
    peao->posTabx = coordx;
    peao->posTaby = coordy;
    peao->simb = simb;
    peao->direc = direct;
    peao->ID = ID;
    peao->sentido = sentido;
}

int lancarDado(){
    printf("\nPressione Enter para rolar o dado... \n");
    getchar();
    int num = (rand() % 6) + 1;
    return num;
}

int jogadorIniciado(Peao peao[4]){
    int estaEmCasa = 0;
    for(int i = 0; i < 4; i++){
        if(peao[i].pos == 0){
            estaEmCasa++;
        }
    }
    return estaEmCasa;
}

void tirardeCasa(Peao *peoes){
    for(int i = 0; i < 4; i++){
        if(peoes[i].pos == 0){
            /*switch (peao->simb){
            case '1':
                peao->sentido = -1;
                peao->direc = 'v';
                break;
            case '2':
                peao->sentido = 1;
                peao->direc = 'v';
                break;
            case '3':
                peao->sentido = 1;
                peao->direc = 'h';
                break;
            case '4':
                peao->sentido = -1;
                peao->direc = 'h';
                break;
            default:
                break;
            }*/
            moverPeao(peoes, 1, i);
            return;
        }
    }
}

int estaEmCasa(Peao peao){
    if(peao.pos == 0){
        return 1;
    }else{
        return 0;
    }
}

int verificaQualPeao(Peao *peao){
    int escolha;
    printf("Qual peão vc deseja mover?\n");
    for (int i = 0; i < 4; i++){
        if(!estaEmCasa(peao[i])){
            printf("%d - Peão na posição %d, direção: %c, sentido: %d, posição no tabuleiro: [%d, %d]\n", 
            peao[i].ID, peao[i].pos, peao->direc, peao->sentido, peao->posTabx, peao->posTaby);
        }
    }
    printf("Sua escolha: ");
    scanf("%d", &escolha);
    return escolha;
}

void Jogar(int dado, Peao *peoes){
    printf("Peão simbolo: %c\n", peoes[0].simb);
    int indice;
    if(jogadorIniciado(peoes) == 4){
        if(dado == 6){
            tirardeCasa(peoes);
        }
    }else{ 
        if(jogadorIniciado(peoes) > 0 && dado == 6){
            char escolha;
            printf("Você tem peões em casa! Deseja tirar um?(s/n)\n");
            scanf("%c", &escolha);
            if (escolha == 's'){
                tirardeCasa(peoes);
            }else{
                indice = verificaQualPeao(peoes);
                moverPeao(peoes, dado, indice);
                printf("Movendo peão: %d \n", peoes[indice].ID);
            }
        }else{
            indice = verificaQualPeao(peoes);
            moverPeao(peoes, dado, indice);
             printf("Movendo peão: %d \n", peoes[indice].ID);
        }
    }
}