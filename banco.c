#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "meu_banco.h"

typedef struct {
    char descricao[50];
    float valor;
    int numeroConta;
} Transacao;

typedef struct {
    int numero;      
    int tipo;        
    float saldo;
    int idCliente;
} Conta;

typedef struct {
    char nome[50];
    int temEmprestimo;   
} Cliente;


Cliente clientes[100];    int nClientes = 0;
Conta contas[200];        int nContas = 0;
Transacao hist[1000];     int nHist = 0;
int proximoNumero = 1000;
int aberto = 0;         

void registrar(char desc[], float valor, int numeroConta) {
    strcpy(hist[nHist].descricao, desc);
    hist[nHist].valor = valor;
    hist[nHist].numeroConta = numeroConta;
    nHist++;
}

int buscarConta(int cliente, int tipo) {
    int i;
    for (i = 0; i < nContas; i++) {
        if (contas[i].idCliente == cliente && contas[i].tipo == tipo)
            return i;
    }
    return -1;
}

int escolherCliente() {
    int i, n;
    if (nClientes == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return -1;
    }
    for (i = 0; i < nClientes; i++)
        printf("%d - %s\n", i + 1, clientes[i].nome);
    printf("Numero do cliente: ");
    scanf("%d", &n);
    if (n < 1 || n > nClientes) {
        printf("Cliente invalido!\n");
        return -1;
    }
    return n - 1;
}

void cadastrarCliente() {
    if (nClientes >= 100) {
        printf("Limite de clientes atingido!\n");
        return;
    }
    printf("Nome (sem espacos): ");
    scanf("%s", clientes[nClientes].nome);
    clientes[nClientes].temEmprestimo = 0;
    nClientes++;
    printf("Cliente cadastrado!\n");
}

void abrirConta() {
    int c, tipo;
    c = escolherCliente();
    if (c < 0) return;

    printf("Tipo (1-Corrente, 2-Salario, 3-Poupanca): ");
    scanf("%d", &tipo);
    if (tipo < 1 || tipo > 3) {
        printf("Tipo invalido!\n");
        return;
    }
    if (buscarConta(c, tipo) >= 0) {
        printf("O cliente ja tem essa conta.\n");
        return;
    }

    if (tipo == 1 && buscarConta(c, 2) >= 0) {
        printf("Cliente com conta salario nao pode ter corrente.\n");
        return;
    }
    if (tipo == 2 && buscarConta(c, 1) >= 0) {
        printf("Cliente com conta corrente nao pode ter salario.\n");
        return;
    }
    if (nContas >= 200) {
        printf("Limite de contas atingido!\n");
        return;
    }

    contas[nContas].numero = proximoNumero;
    contas[nContas].tipo = tipo;
    contas[nContas].saldo = 0;
    contas[nContas].idCliente = c;
    registrar("Abertura de conta", 0, proximoNumero);
    printf("Conta %d-%02d criada!\n", proximoNumero, tipo);
    proximoNumero++;
    nContas++;
}

void depositar() {
    int c, tipo, i;
    float valor;
    c = escolherCliente();
    if (c < 0) return;

    printf("Tipo da conta (1-Corrente, 2-Salario, 3-Poupanca): ");
    scanf("%d", &tipo);
    i = buscarConta(c, tipo);
    if (i < 0) {
        printf("Conta nao existe.\n");
        return;
    }
    printf("Valor: ");
    scanf("%f", &valor);
    if (valor <= 0) {
        printf("Valor invalido!\n");
        return;
    }
    contas[i].saldo = contas[i].saldo + valor;
    registrar("Deposito", valor, contas[i].numero);
    printf("Saldo: R$ %.2f\n", contas[i].saldo);
}

void sacar() {
    int c, tipo, i;
    float valor;
    c = escolherCliente();
    if (c < 0) return;

    printf("Tipo da conta (1-Corrente, 2-Salario, 3-Poupanca): ");
    scanf("%d", &tipo);
    i = buscarConta(c, tipo);
    if (i < 0) {
        printf("Conta nao existe.\n");
        return;
    }
    printf("Valor: ");
    scanf("%f", &valor);
    if (valor <= 0 || valor > contas[i].saldo) {
        printf("Valor invalido ou saldo insuficiente!\n");
        return;
    }
    contas[i].saldo = contas[i].saldo - valor;
    registrar("Saque", -valor, contas[i].numero);
    printf("Saldo: R$ %.2f\n", contas[i].saldo);
}

void emprestimo() {
    int c, corrente, poupanca, i;
    float total = 0, limite, valor;
    c = escolherCliente();
    if (c < 0) return;

    if (clientes[c].temEmprestimo == 1) {
        printf("Cliente ja fez um emprestimo.\n");
        return;
    }
    corrente = buscarConta(c, 1);
    poupanca = buscarConta(c, 3);
    if (corrente < 0 || poupanca < 0) {
        printf("Precisa de conta corrente e poupanca.\n");
        return;
    }
    if (contas[poupanca].saldo <= 0) {
        printf("A poupanca precisa ter saldo.\n");
        return;
    }

    for (i = 0; i < nContas; i++)
        if (contas[i].idCliente == c)
            total = total + contas[i].saldo;

    limite = 0.25 * 10 * total;
    printf("Limite do emprestimo: R$ %.2f\n", limite);
    printf("Valor desejado: ");
    scanf("%f", &valor);
    if (valor <= 0 || valor > limite) {
        printf("Valor invalido ou acima do limite!\n");
        return;
    }
    contas[corrente].saldo = contas[corrente].saldo + valor;
    clientes[c].temEmprestimo = 1;
    registrar("Emprestimo", valor, contas[corrente].numero);
    printf("Emprestimo creditado na conta corrente!\n");
}


void abrirBanco() {
    int op;
    if (aberto == 0) {
        aberto = 1;
        srand(time(NULL));
        printf("Banco aberto!\n");
    }
    do {
        printf("\n--- OPERACOES ---\n");
        printf("1 - Cadastrar cliente\n");
        printf("2 - Abrir conta\n");
        printf("3 - Depositar\n");
        printf("4 - Sacar\n");
        printf("5 - Pedir emprestimo\n");
        printf("0 - Voltar\n");
        printf("Opcao: ");
        scanf("%d", &op);

        if (op == 1) cadastrarCliente();
        else if (op == 2) abrirConta();
        else if (op == 3) depositar();
        else if (op == 4) sacar();
        else if (op == 5) emprestimo();
        else if (op != 0) printf("Opcao invalida!\n");
    } while (op != 0);
}

void consultarHistorico() {
    int c, i, j;
    if (aberto == 0) {
        printf("O banco esta fechado! Abra o banco primeiro.\n");
        return;
    }
    c = escolherCliente();
    if (c < 0) return;

    printf("\nCliente: %s\n", clientes[c].nome);
    for (i = 0; i < nContas; i++) {
        if (contas[i].idCliente == c) {
            printf("\nConta %d-%02d | Saldo: R$ %.2f\n",
                   contas[i].numero, contas[i].tipo, contas[i].saldo);
            for (j = 0; j < nHist; j++)
                if (hist[j].numeroConta == contas[i].numero)
                    printf("  %s: R$ %.2f\n", hist[j].descricao, hist[j].valor);
        }
    }
}

void efetuarSorteio() {
    int c, i;
    if (aberto == 0) {
        printf("O banco esta fechado! Abra o banco primeiro.\n");
        return;
    }
    if (nClientes == 0) {
        printf("Nenhum cliente para sortear.\n");
        return;
    }
    c = rand() % nClientes;
    printf("Sorteado: %s\n", clientes[c].nome);

    for (i = 0; i < nContas; i++) {
        if (contas[i].idCliente == c) {
            contas[i].saldo = contas[i].saldo + 100;
            registrar("Premio do sorteio", 100, contas[i].numero);
            printf("Ganhou R$ 100,00 na conta %d-%02d!\n",
                   contas[i].numero, contas[i].tipo);
            return;
        }
    }
    printf("Ele nao tem conta, ficou sem premio.\n");
}

void liberarBanco() {
    nClientes = 0;
    nContas = 0;
    nHist = 0;
    proximoNumero = 1000;
    aberto = 0;
}