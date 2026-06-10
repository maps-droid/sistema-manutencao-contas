#include <stdio.h>
#include <stdlib.h>

#define ARQUIVO "contas.dat"
#define MAX_REGISTROS 100

typedef struct {
    int conta;
    char nome[50];
    float saldo;
} Cliente;

void cadastrarCliente(FILE *arq);
void consultarCliente(FILE *arq);
void atualizarSaldo(FILE *arq);
void encerrarConta(FILE *arq);
void listarClientes(FILE *arq);
void restaurarLeitura(FILE *arq);

int main() {
    FILE *arq;
    int opcao;

    arq = fopen(ARQUIVO, "r+b");

    if (arq == NULL) {
        arq = fopen(ARQUIVO, "w+b");

        Cliente vazio = {0, "", 0.0};

        for (int i = 0; i < MAX_REGISTROS; i++) {
            fwrite(&vazio, sizeof(Cliente), 1, arq);
        }
    }

    do {
        printf("\n=== SISTEMA DE CONTAS ===\n");
        printf("1. Cadastrar novo cliente\n");
        printf("2. Consultar cliente\n");
        printf("3. Atualizar saldo\n");
        printf("4. Encerrar conta\n");
        printf("5. Listar clientes\n");
        printf("6. Restaurar leitura (rewind)\n");
        printf("7. Encerrar\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                cadastrarCliente(arq);
                break;

            case 2:
                consultarCliente(arq);
                break;

            case 3:
                atualizarSaldo(arq);
                break;

            case 4:
                encerrarConta(arq);
                break;

            case 5:
                listarClientes(arq);
                break;

            case 6:
                restaurarLeitura(arq);
                break;

            case 7:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while(opcao != 7);

    fclose(arq);

    return 0;
}

void cadastrarCliente(FILE *arq) {
    Cliente cliente;
    int posicao;

    printf("Posicao (0 a %d): ", MAX_REGISTROS - 1);
    scanf("%d", &posicao);

    if (posicao < 0 || posicao >= MAX_REGISTROS) {
        printf("Posicao invalida!\n");
        return;
    }

    fseek(arq, posicao * sizeof(Cliente), SEEK_SET);
    fread(&cliente, sizeof(Cliente), 1, arq);

    if (cliente.conta != 0) {
        printf("Posicao ja ocupada!\n");
        return;
    }

    printf("Numero da conta: ");
    scanf("%d", &cliente.conta);

    printf("Nome: ");
    scanf(" %[^\n]", cliente.nome);

    printf("Saldo: ");
    scanf("%f", &cliente.saldo);

    fseek(arq, posicao * sizeof(Cliente), SEEK_SET);
    fwrite(&cliente, sizeof(Cliente), 1, arq);

    fflush(arq);

    printf("Cliente cadastrado com sucesso!\n");
}

void consultarCliente(FILE *arq) {
    Cliente cliente;

    int conta;

    printf("Numero da conta: ");
    scanf("%d", &conta);

    rewind(arq);

    while (fread(&cliente, sizeof(Cliente), 1, arq) == 1) {

        if (cliente.conta == conta) {
            printf("\nConta: %d\n", cliente.conta);
            printf("Nome : %s\n", cliente.nome);
            printf("Saldo: %.2f\n", cliente.saldo);
            return;
        }
    }

    printf("Cliente nao encontrado!\n");
}

void atualizarSaldo(FILE *arq) {
    Cliente cliente;

    int conta;
    float novoSaldo;

    printf("Numero da conta: ");
    scanf("%d", &conta);

    rewind(arq);

    while (fread(&cliente, sizeof(Cliente), 1, arq) == 1) {

        if (cliente.conta == conta) {

            printf("Novo saldo: ");
            scanf("%f", &novoSaldo);

            cliente.saldo = novoSaldo;

            fseek(arq, -sizeof(Cliente), SEEK_CUR);
            fwrite(&cliente, sizeof(Cliente), 1, arq);

            fflush(arq);

            printf("Saldo atualizado!\n");
            return;
        }
    }

    printf("Cliente nao encontrado!\n");
}

void encerrarConta(FILE *arq) {
    Cliente cliente;
    Cliente vazio = {0, "", 0.0};

    int conta;

    printf("Numero da conta: ");
    scanf("%d", &conta);

    rewind(arq);

    while (fread(&cliente, sizeof(Cliente), 1, arq) == 1) {

        if (cliente.conta == conta) {

            fseek(arq, -sizeof(Cliente), SEEK_CUR);
            fwrite(&vazio, sizeof(Cliente), 1, arq);

            fflush(arq);

            printf("Conta encerrada!\n");
            return;
        }
    }

    printf("Cliente nao encontrado!\n");
}

void listarClientes(FILE *arq) {
    Cliente cliente;

    rewind(arq);

    printf("\n=== CLIENTES CADASTRADOS ===\n");

    while (fread(&cliente, sizeof(Cliente), 1, arq) == 1) {

        if (cliente.conta != 0) {
            printf("Conta: %d\n", cliente.conta);
            printf("Nome : %s\n", cliente.nome);
            printf("Saldo: %.2f\n", cliente.saldo);
            printf("-------------------------\n");
        }
    }
}

void restaurarLeitura(FILE *arq) {
    rewind(arq);
    printf("Leitura reposicionada no inicio do arquivo.\n");
}