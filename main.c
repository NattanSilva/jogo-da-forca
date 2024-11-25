#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

// definindo constantes
#define MAX_WORDS 100
#define MAX_NAME 50
#define MAX_WORD 20

/*
Jogo da Forca
a) Ranking em arquivo;
b) Banco de dados de palavras carregadas do arquivo;
c) Deve haver menu com op��es de Jogar, Ver Ranking, Cadastrar Palavras,
Cr�ditos e Sair;
d) Utilize array com aloca��o din�mica baseado na palavra da vez;
e) Utilize struct para armazenar em mem�ria e depois transferir para arquivo;
f) Controle de jogada deve ser feito utilizando entrada de dados pelo teclado
contendo informa��o da posi��o e usu�rio;
*/

// Estrutura para armazenar palavras
typedef struct {
    char palavra[MAX_WORD];
    char dica[MAX_WORD];
} Palavra;

// Estrutura para ranking
typedef struct {
    char nome[MAX_NAME];
    int pontos;
} Ranking;

Palavra palavras[MAX_WORDS];
Ranking ranking[MAX_WORDS];
int total_palavras = 0;
int total_ranking = 0;

// Prototipa��o
void carregarPalavras();
void salvarRanking();
void carregarRanking();
void jogar();
void verRanking();
void cadastrarPalavras();
void exibirCreditos();
void menu();

// Fun��o principal
int main() {
	setlocale(LC_ALL, "Portuguese");
    carregarPalavras();
    carregarRanking();
    menu();
    return 0;
}

// Fun��o para exibir o menu
void menu() {
    int opcao;
    do {
        printf("\n=== JOGO DA FORCA ===\n");
        printf("1. Jogar\n");
        printf("2. Ver Ranking\n");
        printf("3. Cadastrar Palavras\n");
        printf("4. Cr�ditos\n");
        printf("5. Sair\n");
        printf("Escolha uma op��o: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogar();
                break;
            case 2:
                verRanking();
                break;
            case 3:
                cadastrarPalavras();
                break;
            case 4:
                exibirCreditos();
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Op��o inv�lida!\n");
        }
    } while (opcao != 5);
}

// Fun��o para carregar palavras do arquivo
void carregarPalavras() {
	// Abre o arquivo para leitura
    FILE *arquivo = fopen("palavras.txt", "r");
    // Verifica se o arquivo abriu corretamente
    if (!arquivo) {
        printf("Erro ao carregar palavras!\n");
        return;
    }
    total_palavras = 0;
    // L� cada palava do arquivo
    while (fscanf(arquivo, "%s", palavras[total_palavras].palavra) != EOF) {
        total_palavras++;
    }
    // Fecha o arquivo
    fclose(arquivo);
}

// Fun��o para salvar ranking em arquivo
void salvarRanking() {
	// Abre o arquivo para escrita
    FILE *arquivo = fopen("ranking.txt", "w");
    int i = 0;
    // Verifica se o arquivo abriu corretamente
    if (!arquivo) {
        printf("Erro ao salvar ranking!\n");
        return;
    }
    // Escreve os dados do ranking no arquivo
    for (i = 0; i < total_ranking; i++) {
        fprintf(arquivo, "%s %d\n", ranking[i].nome, ranking[i].pontos);
    }
    // Fecha o arquivo
    fclose(arquivo);
}

// Fun��o para carregar ranking do arquivo
void carregarRanking() {
	// Abre o arquivo para leitura
    FILE *arquivo = fopen("ranking.txt", "r");
    // Verifica se o arquivo abriu corretamente
    if (!arquivo) {
        return;
    }
    total_ranking = 0;
    // Ler os dados do arquivo pelo nome e pontua��o
    while (fscanf(arquivo, "%s %d", ranking[total_ranking].nome, &ranking[total_ranking].pontos) != EOF) {
        total_ranking++;
    }
    // fecha o arquivo
    fclose(arquivo);
}

// Fun��o para jogar
void jogar() {
	int i = 0;
	// valida��o se h� palavras cadastradas
    if (total_palavras == 0) {
        printf("Nenhuma palavra cadastrada!\n");
        return;
    }
	// recebe o nome do jogador
    char nome[MAX_NAME];
    printf("Digite seu nome: ");
    scanf("%s", nome);

    // Sele��o aleat�ria da palavra
    int indice = rand() % total_palavras;
    char *palavra = palavras[indice].palavra;
    char *dica = palavras[indice].dica;
	
	// Inicia a m�scara da palavra
    int tamanho = strlen(palavra);
    char *mascara = (char *)malloc(tamanho + 1);
    for (i = 0; i < tamanho; i++) mascara[i] = '_';
    mascara[tamanho] = '\0';

    int erros = 0, acertos = 0;
    char chute;

	// Loop de renderiza��o do jogo
    while (erros < 6 && acertos < tamanho) {
        printf("Palavra: %s\n", mascara);
        printf("Digite uma letra: ");
        scanf(" %c", &chute);
        chute = tolower(chute);

        int acertou = 0;
        // valida��o do chute do jodador, se estiver certo mostra a letra na tela e incrementa o n�mero de acertos
        for (i = 0; i < tamanho; i++) {
            if (palavra[i] == chute && mascara[i] == '_') {
                mascara[i] = chute;
                acertos++;
                acertou = 1;
            }
        }
		// Se o jogador errar o n�mero de erros � incrementado
        if (!acertou) {
            erros++;
            printf("Erros: %d/6\n", erros);
        }
    }

	// Se o jogador acertaar a palavra toda a pontua��o � calculada
    if (acertos == tamanho) {
        printf("Parab�ns, %s! Voc� acertou a palavra: %s\n", nome, palavra);

        // Adicionar ao ranking
        strcpy(ranking[total_ranking].nome, nome);
        ranking[total_ranking].pontos = tamanho * 10 - erros * 2;
        total_ranking++;
        salvarRanking();
    } else {
    	// caso erre 6 vezes o jogo acaba
        printf("Voc� perdeu! A palavra era: %s\n", palavra);
    }

    free(mascara);
}

// Fun��o para exibir ranking
void verRanking() {
	int i = 0;
    printf("\n=== RANKING ===\n");
	// Percorre o ranking listando o nome do jogador e a pontua��o dele
    for (i = 0; i < total_ranking; i++) {
        printf("%d. %s - %d pontos\n", i + 1, ranking[i].nome, ranking[i].pontos);
    }
}

// Fun��o para cadastrar palavras
void cadastrarPalavras() {
	// Verifica o tamanho do banco de dados caso esteja cheio
    if (total_palavras >= MAX_WORDS) {
        printf("Banco de palavras cheio!\n");
        return;
    }
	// Recebe a nova palavra a ser cadastrada
    printf("Digite a palavra: ");
    scanf("%s", palavras[total_palavras].palavra);
    total_palavras++;

	// Abre o arquivo para salvar a palavra nele
    FILE *arquivo = fopen("palavras.txt", "a");
    // Caso haja algum erro no arquivo mostra um erro e encerra
    if (!arquivo) {
        printf("Erro ao salvar palavra!\n");
        return;
    }
    // Escreve no arquivo a palvra cadastrada
    fprintf(arquivo, "%s\n", palavras[total_palavras - 1].palavra);
    // Fecha o arquivo
    fclose(arquivo);
    // Retorna a mensagem de sucesso para o usu�rio
    printf("Palavra cadastrada com sucesso!\n");
}

// Fun��o para exibir cr�ditos
void exibirCreditos() {
	int total_nomes = 5; // N�mero de nomes atualmente armazenados
	int i = 0;
	char *desenvolvedores[] = {"Natan", "Eduardo", "Derek", "Douglas", "Alawander"};
	
    printf("\n=== CR�DITOS ===\n");
    printf("Desenvolvido por:\n");
    // Mostrando os nomes dos desenvolvedores
    for (i = 0; i < total_nomes; i++) {
        printf("%s\n", desenvolvedores[i]);
    }
    printf("Jogo da Forca em C.\n");
}

