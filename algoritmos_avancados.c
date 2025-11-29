/*Requisitos funcionais
Seu programa em C deverá:
Criar uma árvore binária para representar o mapa da mansão.
     
Permitir a exploração interativa da mansão a partir do Hall de entrada, escolhendo ir para a esquerda (e) ou para a direita (d).
     
Além disso, a estrutura da mansão já vem definida no código, e não é necessário inseri-la manualmente — afinal, ela é criada de modo automático pela função main(), usando a função criarSala().
 
Seu programa em C ainda deverá viabilizar a exploração continua até o jogador alcançar um cômodo que não possua caminhos à esquerda nem à direita (isto é, um nó-folha na árvore). Por fim, o programa exibe o nome de cada sala visitada durante a exploração.

Cada cômodo possui:
nome: uma string que identifica a sala (por exemplo: Sala de estar, Cozinha ou Jardim).

    Árvore binária: estrutura de dados hierárquica com dois filhos por nó.
     
    Structs: Criação de um tipo personalizado (Sala) com campos para nome e ponteiros.
     
    Alocação dinâmica: uso de malloc para criação de nós da árvore.
     
    Operadores condicionais: controle das decisões do jogador (if, else).
     
    Modularização: separação de funcionalidades em funções distintas, com responsabilidades claras.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --------------------- STRUCT DO NÓ DO MAPA --------------------------
typedef struct NoMapa {
    char nome[40];
    char pista[120];
    struct NoMapa *esq;
    struct NoMapa *dir;
} NoMapa;

// --------------------- STRUCT DA ÁRVORE DE PISTAS ---------------------
typedef struct NoPista {
    char pista[120];
    struct NoPista *esq, *dir;
} NoPista;

// -------------------- CRIA NÓ DE PISTA --------------------------------
NoPista* criarNoPista(const char *texto) {
    NoPista *novo = (NoPista *)malloc(sizeof(NoPista));
    strcpy(novo->pista, texto);
    novo->esq = novo->dir = NULL;
    return novo;
}

// -------------------- INSERE PISTA (BST ALFABÉTICA) -------------------
NoPista* inserirPista(NoPista *raiz, const char *texto) {
    if (raiz == NULL)
        return criarNoPista(texto);

    int cmp = strcmp(texto, raiz->pista);

    if (cmp < 0)
        raiz->esq = inserirPista(raiz->esq, texto);
    else if (cmp > 0)
        raiz->dir = inserirPista(raiz->dir, texto);
    // se for igual, não insere (evita duplicata)

    return raiz;
}

// -------------------- IMPRIME PISTAS EM ORDEM ALFABÉTICA --------------
void imprimirPistas(NoPista *raiz) {
    if (!raiz) return;

    imprimirPistas(raiz->esq);
    printf("- %s\n", raiz->pista);
    imprimirPistas(raiz->dir);
}

// -------------------- CRIA NÓ DA ÁRVORE DO MAPA -----------------------
NoMapa* criarNo(const char *nome, const char *pista) {
    NoMapa *novo = (NoMapa* ) malloc(sizeof(NoMapa));
    strcpy(novo->nome, nome);
    strcpy(novo->pista, pista);
    novo->esq = novo->dir = NULL;
    return novo;
}

// -------------------- CRIA O MAPA COMPLETO ----------------------------
NoMapa* criarMapa() {
    NoMapa *hall = criarNo(
        "Hall de Entrada",
        "Marcas de passos no tapete."
    );

    hall->esq = criarNo(
        "Sala de Jantar",
        "Copo quebrado no chão."
    );
    hall->esq->esq = criarNo(
        "Cozinha",
        "Geladeira aberta com comida caída."
    );
    hall->esq->dir = criarNo(
        "Porão",
        "Caixa quebrada com documentos."
    );

    hall->dir = criarNo(
        "Escritório",
        "Caderno com anotações estranhas."
    );
    hall->dir->esq = criarNo(
        "Biblioteca",
        "Livro marcado com página suspeita."
    );
    hall->dir->dir = criarNo(
        "Quarto Secreto",
        "Chave antiga sobre a mesa."
    );

    return hall;
}

// -------------------- EXPLORAÇÃO --------------------------------------
void explorar(NoMapa *atual, NoPista **registro) {
    int escolha;

    while (1) {
        printf("\n==============================\n");
        printf("Você está em: ** %s **\n", atual->nome);

        // REGISTRA A PISTA NA BST
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *registro = inserirPista(*registro, atual->pista);
        }

        if (!atual->esq && !atual->dir) {
            printf("\nNão há saídas. Você chegou ao fim.\n");
            return;
        }

        printf("\nPara onde deseja ir?\n");
        if (atual->esq)
            printf("1 - Esquerda (%s)\n", atual->esq->nome);
        if (atual->dir)
            printf("2 - Direita (%s)\n", atual->dir->nome);
        printf("0 - Sair\nEscolha: ");

        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                if (atual->esq) atual = atual->esq;
                break;

            case 2:
                if (atual->dir) atual = atual->dir;
                break;

            case 0:
                printf("Saindo da exploração...\n");
                return;

            default:
                printf("Opção inválida!\n");
        }
    }
}

// -------------------- MAIN --------------------------------------------
int main() {
    NoMapa *raiz = criarMapa();
    NoPista *registroPistas = NULL;

    printf("============================================\n");
    printf("     Detective Quest - Exploração da Mansão \n");
    printf("============================================\n\n");

    explorar(raiz, &registroPistas);

    printf("\n============================================\n");
    printf("          Pistas encontradas (A-Z)\n");
    printf("============================================\n");

    imprimirPistas(registroPistas);

    return 0;
}
