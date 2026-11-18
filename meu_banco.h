struct transacao{
    char descricao[50];
    float valor;
    int numeroConta;
}

struct conta{
    int numero;
    int tipo;
    float saldo;
    int IdCliente;
}

struct cliente{
    char nome[50];
    struct cliente[3];
    int temEmprestimo;
}
