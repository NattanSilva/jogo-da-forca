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
c) Deve haver menu com opções de Jogar, Ver Ranking, Cadastrar Palavras,
Créditos e Sair;
d) Utilize array com alocação dinâmica baseado na palavra da vez;
e) Utilize struct para armazenar em memória e depois transferir para arquivo;
f) Controle de jogada deve ser feito utilizando entrada de dados pelo teclado
contendo informação da posição e usuário;
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

// Prototipação
void carregarPalavras();
void salvarRanking();
void carregarRanking();
void jogar();
void verRanking();
void cadastrarPalavras();
void exibirCreditos();
void menu();

// Função principal
int main() {
	setlocale(LC_ALL, "Portuguese");
    carregarPalavras();
    carregarRanking();
    menu();
    return 0;
}

// Função para exibir o menu
void menu() {
    int opcao;
    do {
        printf("\n=== JOGO DA FORCA ===\n");
        printf("1. Jogar\n");
        printf("2. Ver Ranking\n");
        printf("3. Cadastrar Palavras\n");
        printf("4. Créditos\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
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
                printf("Opção inválida!\n");
        }
    } while (opcao != 5);
}

// Função para carregar palavras do arquivo
void carregarPalavras() {
	// Abre o arquivo para leitura
    FILE *arquivo = fopen("palavras.txt", "r");
    // Verifica se o arquivo abriu corretamente
    if (!arquivo) {
        printf("Erro ao carregar palavras!\n");
        return;
    }
    total_palavras = 0;
    // Lê cada palava do arquivo
    while (fscanf(arquivo, "%s", palavras[total_palavras].palavra) != EOF) {
        total_palavras++;
    }
    // Fecha o arquivo
    fclose(arquivo);
}

// Função para salvar ranking em arquivo
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

// Função para carregar ranking do arquivo
void carregarRanking() {
	// Abre o arquivo para leitura
    FILE *arquivo = fopen("ranking.txt", "r");
    // Verifica se o arquivo abriu corretamente
    if (!arquivo) {
        return;
    }
    total_ranking = 0;
    // Ler os dados do arquivo pelo nome e pontuação
    while (fscanf(arquivo, "%s %d", ranking[total_ranking].nome, &ranking[total_ranking].pontos) != EOF) {
        total_ranking++;
    }
    // fecha o arquivo
    fclose(arquivo);
}

// Função para jogar
void jogar() {
	int i = 0;
	// validação se há palavras cadastradas
    if (total_palavras == 0) {
        printf("Nenhuma palavra cadastrada!\n");
        return;
    }
	// recebe o nome do jogador
    char nome[MAX_NAME];
    printf("Digite seu nome: ");
    scanf("%s", nome);

    // Seleção aleatória da palavra
    int indice = rand() % total_palavras;
    char *palavra = palavras[indice].palavra;
    char *dica = palavras[indice].dica;
	
	// Inicia a máscara da palavra
    int tamanho = strlen(palavra);
    char *mascara = (char *)malloc(tamanho + 1);
    for (i = 0; i < tamanho; i++) mascara[i] = '_';
    mascara[tamanho] = '\0';

    int erros = 0, acertos = 0;
    char chute;

	// Loop de renderização do jogo
    while (erros < 6 && acertos < tamanho) {
        printf("Palavra: %s\n", mascara);
        printf("Digite uma letra: ");
        scanf(" %c", &chute);
        chute = tolower(chute);

        int acertou = 0;
        // validação do chute do jodador, se estiver certo mostra a letra na tela e incrementa o número de acertos
        for (i = 0; i < tamanho; i++) {
            if (palavra[i] == chute && mascara[i] == '_') {
                mascara[i] = chute;
                acertos++;
                acertou = 1;
            }
        }
		// Se o jogador errar o número de erros é incrementado
        if (!acertou) {
            erros++;
            printf("Erros: %d/6\n", erros);
        }
    }

	// Se o jogador acertaar a palavra toda a pontuação é calculada
    if (acertos == tamanho) {
        printf("Parabéns, %s! Você acertou a palavra: %s\n", nome, palavra);

        // Adicionar ao ranking
        strcpy(ranking[total_ranking].nome, nome);
        ranking[total_ranking].pontos = tamanho * 10 - erros * 2;
        total_ranking++;
        salvarRanking();
    } else {
    	// caso erre 6 vezes o jogo acaba
        printf("Você perdeu! A palavra era: %s\n", palavra);
    }

    free(mascara);
}

// Função para exibir ranking
void verRanking() {
	int i = 0;
    printf("\n=== RANKING ===\n");
	// Percorre o ranking listando o nome do jogador e a pontuação dele
    for (i = 0; i < total_ranking; i++) {
        printf("%d. %s - %d pontos\n", i + 1, ranking[i].nome, ranking[i].pontos);
    }
}

// Função para cadastrar palavras
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
    // Retorna a mensagem de sucesso para o usuário
    printf("Palavra cadastrada com sucesso!\n");
}

// Função para exibir créditos
void exibirCreditos() {
	int total_nomes = 5; // Número de nomes atualmente armazenados
	int i = 0;
	char *desenvolvedores[] = {"Natan", "Eduardo", "Derek", "Douglas", "Alawander"};
	
    printf("\n=== CRÉDITOS ===\n");
    printf("Desenvolvido por:\n");
    // Mostrando os nomes dos desenvolvedores
    for (i = 0; i < total_nomes; i++) {
        printf("%s\n", desenvolvedores[i]);
    }
    printf("Jogo da Forca em C.\n");
}

