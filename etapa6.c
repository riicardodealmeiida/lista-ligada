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
        printf("[int] %d\n", *(int *)no->dados);
    } else if (no->tipo == 2) {
        printf("[float] %.2f\n", *(float *)no->dados);
    } else {
        printf("[string] %s\n", (char *)no->dados);
    }
}
void imprimir(No *lista) {
    int i = 0;
    if (lista == NULL) printf("Lista vazia\n");
    while (lista != NULL) {
        printf("%d: ", i);
        imprimirNo(lista);
        lista = lista->proximo;
        i++;
    }
}
int buscar(No *lista, int tipo, void *dados) {
    int i = 0;
    while (lista != NULL) {
        if (lista->tipo == tipo) {
            if (tipo == 1 && *(int *)lista->dados == *(int *)dados) return i;
            if (tipo == 2 && *(float *)lista->dados == *(float *)dados) return i;
            if (tipo == 3 && strcmp((char *)lista->dados, (char *)dados) == 0) return i;
        }
        lista = lista->proximo;
        i++;
    }
    return -1;
}
int remover(No **lista, int pos) {
    No *atual = *lista;
    No *anterior = NULL;
    int i = 0;
    while (atual != NULL && i < pos) {
        anterior = atual;
        atual = atual->proximo;
        i++;
    }
    if (atual == NULL || pos < 0) return 0;
    if (anterior == NULL) {
        *lista = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }
    free(atual->dados);
    free(atual);
    return 1;
}
void salvar(No *lista) {
    FILE *arquivo = fopen("lista.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo\n");
        return;
    }
    while (lista != NULL) {
        int tam = tamanho(lista->tipo, lista->dados);
        fwrite(&lista->tipo, sizeof(int), 1, arquivo);
        fwrite(&tam, sizeof(int), 1, arquivo);
        fwrite(lista->dados, tam, 1, arquivo);
        lista = lista->proximo;
    }
    fclose(arquivo);
    printf("Lista salva\n");
}
void liberar(No **lista) {
    while (*lista != NULL) {
        remover(lista, 0);
    }
}
void carregar(No **lista) {
    int tipo, tam;
    FILE *arquivo = fopen("lista.bin", "rb");
    if (arquivo == NULL) {
        printf("Arquivo nao encontrado\n");
        return;
    }
    liberar(lista);
    while (fread(&tipo, sizeof(int), 1, arquivo) == 1 &&
           fread(&tam, sizeof(int), 1, arquivo) == 1) {
        void *dados = malloc(tam);
        if (fread(dados, tam, 1, arquivo) != 1) {
            free(dados);
            break;
        }
        inserir(lista, tipo, dados, tam);
        free(dados);
    }
    fclose(arquivo);
    printf("Lista carregada\n");
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
    int op, tipo, tam, pos;
    char dados[100];
    do {
        printf("\n1-Inserir 2-Remover 3-Buscar 4-Imprimir 5-Salvar 6-Carregar 0-Sair\n");
        printf("Opcao: ");
        if (scanf("%d", &op) != 1) break;
        if (op == 1 || op == 3) {
            printf("Tipo (1-int 2-float 3-string): ");
            scanf("%d", &tipo);
            if (tipo < 1 || tipo > 3) {
                printf("Tipo invalido\n");
                continue;
            }
            tam = lerDado(tipo, dados);
            if (tam == 0) {
                printf("Entrada invalida\n");
                break;
            }
            if (op == 1) {
                inserir(&lista, tipo, dados, tam);
            } else {
                pos = buscar(lista, tipo, dados);
                if (pos >= 0) printf("Encontrado na posicao %d\n", pos);
                else printf("Nao encontrado\n");
            }
        } else if (op == 2) {
            printf("Posicao: ");
            scanf("%d", &pos);
            if (remover(&lista, pos)) printf("Removido\n");
            else printf("Posicao invalida\n");
        } else if (op == 4) {
            imprimir(lista);
        } else if (op == 5) {
            salvar(lista);
        } else if (op == 6) {
            carregar(&lista);
        }
    } while (op != 0);
    liberar(&lista);
    return 0;
}
