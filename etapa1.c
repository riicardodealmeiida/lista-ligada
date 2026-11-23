#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
    struct No *proximo;
    int tipo;
    void *dados;
} No;

int tamanho(int tipo, void *dados) {
    if (tipo == 1) return sizeof(int);
    if (tipo == 2) return sizeof(float);
    return strlen((char *)dados) + 1;
}

void inserir(No **lista, int tipo, void *dados, int tam) {
    No *novo = malloc(sizeof(No));
    novo->tipo = tipo;
    novo->dados = malloc(tam);
    memcpy(novo->dados, dados, tam);
    novo->proximo = NULL;

    if (*lista == NULL) {
        *lista = novo;
    } else {
        No *aux = *lista;
        while (aux->proximo != NULL) {
            aux = aux->proximo;
        }
        aux->proximo = novo;
    }
}

void imprimirNo(No *no) {
    if (no->tipo == 1) {
        printf("[int] %d\\n", *(int *)no->dados);
    } else if (no->tipo == 2) {
        printf("[float] %.2f\\n", *(float *)no->dados);
    } else {
        printf("[string] %s\\n", (char *)no->dados);
    }
}

void imprimir(No *lista) {
    int i = 0;

    if (lista == NULL) printf("Lista vazia\\n");

    while (lista != NULL) {
        printf("%d: ", i);
        imprimirNo(lista);
        lista = lista->proximo;
        i++;
    }
}

int lerDado(int tipo, void *dados) {
    if (tipo == 1) {
        printf("Valor: ");
        if (scanf("%d", (int *)dados) != 1) return 0;
        return sizeof(int);
    }

    if (tipo == 2) {
        printf("Valor: ");
        if (scanf("%f", (float *)dados) != 1) return 0;
        return sizeof(float);
    }

    printf("Texto (sem espacos): ");
    if (scanf("%99s", (char *)dados) != 1) return 0;
    return strlen((char *)dados) + 1;
}

int main() {
    No *lista = NULL;
    int op, tipo, tam;
    char dados[100];

    do {
        printf("\\n1-Inserir 2-Imprimir 0-Sair\\n");
        printf("Opcao: ");
        scanf("%d", &op);

        if (op == 1) {
            printf("Tipo (1-int 2-float 3-string): ");
            scanf("%d", &tipo);
            if (tipo >= 1 && tipo <= 3) {
                tam = lerDado(tipo, dados);
                inserir(&lista, tipo, dados, tam);
            } else {
                printf("Tipo invalido\\n");
            }
        } else if (op == 2) {
            imprimir(lista);
        }
    } while (op != 0);

    return 0;
}
