/**
 * Autores: Gabriel de Resende Oliveira Marques e Igor Borges Kapitzky;
 * Disciplina: Programa��o 1 - 2024/2;
 * Trabalho pr�tico;
 * Programa que simula um ca�a palavras est�tico para dois jogadores;
 */

// Inclus�o de bibliotecas;
#include <stdio.h>
#include <string.h>        // Precisamos para a fun��o strlen
#include <stdlib.h>        // Precisamos para as fun��es system e exit
#include <time.h>          // Precisamos para a fun��o srand e rand

// Defini��o da estrutura tJogadores;
typedef struct{

    char nome[15];
    int pontuacao;

}tJogadores;

// Fun��o que cadastra os jogadores;
void cadastrarJogadores(tJogadores jogadores[2]) {  

    printf("------------------------------\n");
    printf("\nDigite o nome dos jogadores: \n");
    for(int i = 0; i < 2; i++){
        scanf("%s", jogadores[i].nome);
    }
    printf("------------------------------");
}

// Fun��o que gera o tabuleiro a partir de uma semente;
void gerarTabuleiro (char tabuleiro[10][10]) {
    printf("------------------------------\n");

    int semente;

    printf("Digite uma \"semente\" (int): \n");
    scanf("%d", &semente);

    srand(semente);                               // Garante que uma mesma semente gere o mesmo tabuleiro todas as vezes;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            tabuleiro[i][j] = 'A' + (rand() % 26);   // A fun��o rand() gera um n�mero aleat�rio e o operador % 26 garante que o n�mero gerado
                                                     // esteja entre 0 e 25, que somados a 'A' resultam em letras do alfabeto na tabela ASCII;
        }
    }

    printf("Tabuleiro gerado para a semente %d\n", semente);
    printf("------------------------------");   
}

// Fun��o que salva o tabuleiro em um arquivo;
void salvarTabuleiro(char tabuleiro[10][10]) {
    char nome_arquivo[50];
    printf("Digite o nome do arquivo: ");
    scanf("%s", nome_arquivo);

    FILE *arquivo = fopen (nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita\n");
        exit(1);
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            fprintf(arquivo, "%c ", tabuleiro[i][j]);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    printf("Tabuleiro salvo no arquivo %s\n", nome_arquivo);
    printf("------------------------------");
}

// Fun��o que carrega o tabuleiro de um arquivo;
void carregarTabuleiro(char tabuleiro[10][10]) {
    char nome_arquivo[50];
    printf("Opcao 4 - Carregar Tabuleiro selecionada.\n");
    printf("Digite o nome do arquivo: ");
    scanf("%s", nome_arquivo);

    FILE *arquivo = fopen (nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para leitura!\n");
        return;
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            fscanf(arquivo, "%c ", &tabuleiro[i][j]);
        }
    }
    fclose(arquivo);
    printf("Tabuleiro carregado do arquivo %s\n", nome_arquivo);
}

// Fun��o que imprime o tabuleiro no terminal;
void imprimirTabuleiro(char tabuleiro[10][10]) {
    printf("Opcao 5 - Imprimir Tabuleiro selecionada.\n");

    printf("------------------------------\n");

    printf("\n        CACA PALAVRAS:\n\n");

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            printf(" %c ", tabuleiro[i][j]); 
        }
        printf("\n"); 
    printf("\n");
    }
}

// Fun��o que busca palavras no tabuleiro na horizontal;
int buscarHorizontal(char tabuleiro[10][10], char palavra[], int *linha, int *coluna) {
    int tamanho_palavra = strlen(palavra);
    
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j <= 10 - tamanho_palavra; j++) {
            int palavra_encontrada = 1;
            for (int k = 0; k < tamanho_palavra; k++) {
                if (tabuleiro[i][j + k] != palavra[k]) {
                    palavra_encontrada = 0;
                    break;
                }
            }
            if (palavra_encontrada) {
                *linha = i;
                *coluna = j;
                return 1; 
            }
        }
    }
    return 0; 
}

// Fun��o que busca palavras no tabuleiro na vertical;
int buscarVertical(char tabuleiro[10][10], char palavra[], int *linha, int *coluna) {
    int tamanho_palavra = strlen(palavra);

    for (int i = 0; i <= 10 - tamanho_palavra; i++) {
        for (int j = 0; j < 10; j++) {
            int palavra_encontrada = 1;
            for (int k = 0; k < tamanho_palavra; k++) {
                if (tabuleiro[i + k][j] != palavra[k]) {
                    palavra_encontrada = 0;
                    break;
                }
            }
            if (palavra_encontrada) {
                *linha = i;
                *coluna = j;
                return 1;
            }
        }
    }
    return 0;
}

// Fun��o que busca palavras no tabuleiro na diagonal;
int buscarDiagonal(char tabuleiro[10][10], char palavra[], int *linha, int *coluna) {
    int tamanho_palavra = strlen(palavra);

    for (int i = 0; i <= 10 - tamanho_palavra; i++) {
        for (int j = 0; j <= 10 - tamanho_palavra; j++) {
            int palavra_encontrada = 1;
            for (int k = 0; k < tamanho_palavra; k++) {
                if (tabuleiro[i + k][j + k] != palavra[k]) {
                    palavra_encontrada = 0;
                    break;
                }
            }
            if (palavra_encontrada) {
                *linha = i;
                *coluna = j;
                return 1;
            }
        }
    }
    return 0;
}

// Fun��o que simula o jogo;
void jogar(tJogadores jogadores[2], char tabuleiro[10][10]) {
    char palavra[10];
    int jogadorAtual = 0; 
    char historico_palavras[30][15];        // Array para armazenar palavras j� encontradas;
    int total_palavras_encontradas = 0;     // Contador de palavras no hist�rico;

    printf("\nInicio do jogo! Digite palavras para buscar no tabuleiro.\n");
    printf("Digite '.' para encerrar o jogo.\n");

    do {
        printf("\n%s, digite uma palavra: ", jogadores[jogadorAtual].nome);
        scanf("%s", palavra);

        if (palavra[0] == '.') {
            printf("\nJogo interrompido.\n");
            printf("------------------------------\n");
            break;
        }

        // Verifica se a palavra já foi encontrada antes;
        int repetida = 0;
        for (int i = 0; i < total_palavras_encontradas; i++) {
            if (strcmp(historico_palavras[i], palavra) == 0) {      // Se a palavra já foi encontrada;
                repetida = 1;
                break;
            }
        }

        if (repetida) {
            printf("Palavra ja foi encontrada antes! Nao sera contada novamente.\n");
        } else {
            int linha, coluna;
            int palavra_encontrada = buscarHorizontal(tabuleiro, palavra, &linha, &coluna) ||   //Utilizamos o operador lógico || para verificar se a palavra foi encontrada em alguma direção;
                                    buscarVertical(tabuleiro, palavra, &linha, &coluna) ||      //Caso tenha sido, atribuimos 1 a variável palavra_encontrada;
                                    buscarDiagonal(tabuleiro, palavra, &linha, &coluna);

            if (palavra_encontrada) {
                printf("Palavra encontrada em (%d, %d)\n", linha + 1, coluna + 1);      //Se palavra_encontrada for 1, a palavra foi encontrada e imprimimos a posição;
                jogadores[jogadorAtual].pontuacao += 1;

                // Adiciona a palavra ao histórico;
                if (total_palavras_encontradas < 30) {      // Só armazena até 30 palavras;
                    strcpy(historico_palavras[total_palavras_encontradas], palavra);
                    total_palavras_encontradas++;
                }
            } else {
                printf("Palavra nao encontrada.\n");                                       //Se palavra_encontrada for 0, a palavra não foi encontrada e decrementamos a pontuação;
                if (jogadores[jogadorAtual].pontuacao > 0) {    // Garante que não fique negativo;
                    jogadores[jogadorAtual].pontuacao -= 1;
                }
            }
        }

        printf("%s possui %d pontos.\n", jogadores[jogadorAtual].nome, jogadores[jogadorAtual].pontuacao);

        jogadorAtual = 1 - jogadorAtual; // Alterna entre jogadores;
    } while (palavra[0] != '.');
}

// Fun��o que encerra o jogo e imprime o vencedor, o perdedor e suas pontua��es;
void encerrarJogo(tJogadores jogadores[2], int jogadorAtual, int *verificar_opcao) {

    printf("------------------------------\n");

    if (jogadores[jogadorAtual].pontuacao == 1) {
        printf ("\n%s e o vencedor com %d ponto.", jogadores[jogadorAtual].nome, jogadores[jogadorAtual].pontuacao);
    } else {
        printf ("\n%s e o vencedor com %d pontos.", jogadores[jogadorAtual].nome, jogadores[jogadorAtual].pontuacao);
    }

    if (jogadores[1 - jogadorAtual].pontuacao == 1) {
        printf ("\n%s possui %d ponto.", jogadores[1 - jogadorAtual].nome, jogadores[1 - jogadorAtual].pontuacao);
    } else {
        printf ("\n%s possui %d pontos.", jogadores[1 - jogadorAtual].nome, jogadores[1 - jogadorAtual].pontuacao);
    }
    char opcao;
    printf ("\nDeseja realmente encerrar o jogo? (s-sim ou n-nao): ");
    getchar();
    scanf ("%c", &opcao);

    if (opcao == 's' || opcao == 'S') {
        printf ("\nJogo encerrado!\n");
        *verificar_opcao = 1;
    } else {
        *verificar_opcao = 0;
    }
}

// Declara��o da fun��o principal;
int main() {

    // Declara��o de vari�veis;
    tJogadores jogadores[2];
    jogadores[0].pontuacao = 0;     // Inicializa a pontua��o dos jogadores;
    jogadores[1].pontuacao = 0;     // Inicializa a pontua��o dos jogadores;
    char tabuleiro[10][10];
    int opcao;
    int verificar_opcao = 0;

    // La�o de repeti��o que exibe o menu de op��es at� que o usu�rio escolha encerrar o jogo;
    do {
        printf("\n1 - Cadastrar Jogadores\n");
        printf("2 - Gerar Tabuleiro\n");
        printf("3 - Salvar Tabuleiro\n");
        printf("4 - Carregar Tabuleiro\n");
        printf("5 - Imprimir Tabuleiro\n");
        printf("6 - Jogar\n");
        printf("0 - Encerrar Jogo\n");

        // Solicita ao usu�rio que escolha uma op��o;
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);

        // Estrutura de decis�o que chama a fun��o correspondente � op��o escolhida pelo usu�rio;
        switch (opcao) {
            case 1:
                system("cls");                      // Limpa a tela do terminal;
                cadastrarJogadores(jogadores);      // Chama a fun��o que cadastra os jogadores usando a vari�vel jogadores como argumento;
                break;
            case 2:
                system("cls");
                gerarTabuleiro(tabuleiro);          // Chama a fun��o que gera o tabuleiro usando a vari�vel tabuleiro como argumento;
                break;
            case 3:
                system("cls");
                salvarTabuleiro(tabuleiro);         // Chama a fun��o que salva o tabuleiro em um arquivo usando a vari�vel tabuleiro como argumento;
                break;
            case 4:
                system("cls");
                carregarTabuleiro(tabuleiro);       // Chama a fun��o que carrega o tabuleiro de um arquivo usando a vari�vel tabuleiro como argumento;
                break;
            case 5:
                system("cls");
                imprimirTabuleiro(tabuleiro);       // Chama a fun��o que imprime o tabuleiro no terminal usando a vari�vel tabuleiro como argumento;
                break;
            case 6:
                jogar(jogadores, tabuleiro);        // Chama a fun��o que simula o jogo usando as vari�veis jogadores e tabuleiro como argumentos;
                break;
            case 0:
                system("cls");
                encerrarJogo(jogadores, 0, &verificar_opcao);   // Chama a fun��o que encerra o jogo e imprime o vencedor e o perdedor usando as vari�veis jogadores e verificar_opcao como argumentos;
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (verificar_opcao == 0);

    // Retorna 0 para o sistema operacional indicando que o programa foi executado sem erros;
    return 0;
}

