#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED "\x1b[31m"
#define RESET "\x1b[0m"
#define GREEN "\x1b[32m"

// Declaração dos structs
struct cliente {
  char nome[50];
  char sexo;
  int idade;
};

typedef struct cliente cliente;

struct venda {
  int itens;
  int num_sabores;
  float valor_total;
  cliente cliente;
};

typedef struct venda venda;

void limpar_buffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

// Declaração das funções do programa
void cadastrar_cliente(cliente *c);
void cadastrar_venda(venda *v, FILE *file);
void pesquisar_vendas_de_cliente(venda *vendas, int num_vendas);
void carregar_todas_vendas(venda *vendas, int qtd);
int carregar_vendas_arquivo(venda **vendas);

// Menu
int main(void) {
  FILE *file;

  file = fopen("registros.txt", "a+");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo\n");
    return 1;
  }

  int seletor;
  int num_vendas = 0;
  venda *vendas = NULL;

  int vendas_carregadas = carregar_vendas_arquivo(&vendas);
  num_vendas += vendas_carregadas;

  do {
    printf("\n ###       Bem-Vindo ao Dev Gelados       ###\n\n");
    printf(" # 1) Cadastrar novas vendas                #\n");
    printf(" # 2) Visualizar venda através de cliente   #\n");
    printf(" # 3) Ver informação de todas as vendas     #\n");
    printf(" # 4) Encerrar programa                     #\n");
    printf("\nDigite a opção desejada: ");
    scanf("%d", &seletor);
    limpar_buffer();

    switch (seletor) {
    case 1: {
      int num_novas_vendas;
      printf("Digite a quantidade de vendas que deseja cadastrar: ");
      scanf("%d", &num_novas_vendas);
      limpar_buffer();

      vendas = (venda *)realloc(vendas, (num_vendas + num_novas_vendas) *
                                            sizeof(venda));
      if (vendas == NULL) {
        printf(RED "Erro ao alocar memória\n" RESET);
        return 1;
      }

      for (int i = 0; i < num_novas_vendas; i++) {
        printf("\nCadastro da venda %d\n", i + 1);
        cadastrar_cliente(&vendas[num_vendas + i].cliente);
        cadastrar_venda(&vendas[num_vendas + i], file);

        fprintf(file, "Cliente: %s\n", vendas[num_vendas + i].cliente.nome);
        fprintf(file, "Sexo do cliente: %c\n",
                vendas[num_vendas + i].cliente.sexo);
        fprintf(file, "Idade do cliente: %d\n",
                vendas[num_vendas + i].cliente.idade);
        fprintf(file, "Número de itens: %d\n", vendas[num_vendas + i].itens);
        fprintf(file, "Número de sabores: %d\n",
                vendas[num_vendas + i].num_sabores);
        fprintf(file, "Valor total: %.2f\n",
                vendas[num_vendas + i].valor_total);
        fprintf(file, "--------------------------\n");
      }

      num_vendas += num_novas_vendas;
      printf(GREEN "\nCadastro realizado com sucesso!\n" RESET);
      break;
    }
    case 2:
      pesquisar_vendas_de_cliente(vendas, num_vendas);
      break;
    case 3:
      carregar_todas_vendas(vendas, num_vendas);
      break;
    case 4:
      printf("Programa encerrando...\n");
      printf("Programa encerrado.\n");
      break;
    default:
      printf(RED "Opção inválida\n" RESET);
      break;
    }

  } while (seletor != 4);

  fclose(file);
  free(vendas);
  return 0;
}

// Função para cadastrar os clientes
void cadastrar_cliente(cliente *c) {
  char buffer[100];
  int validar;
  char ch;

  printf("Digite o nome do cliente: ");
  fgets(c->nome, sizeof(c->nome), stdin);
  c->nome[strcspn(c->nome, "\n")] = '\0';
  while (strlen(c->nome) < 3) {
    printf(RED "\nErro: Nome inválido, o nome do cliente deve possuir no "
               "mínimo 3 letras.\n" RESET);
    printf("Digite o nome do cliente novamente: ");
    fgets(c->nome, sizeof(c->nome), stdin);
    c->nome[strcspn(c->nome, "\n")] = '\0';
  }

  do {
    printf("Digite o sexo do cliente (M/F/N): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 1 &&
        (buffer[0] == 'M' || buffer[0] == 'm' || buffer[0] == 'F' ||
         buffer[0] == 'f' || buffer[0] == 'N' || buffer[0] == 'n')) {
      c->sexo = buffer[0];
      validar = 1;
    } else {
      printf(RED "\nErro: Sexo inválido, M - Masculino, F - Feminino ou N - "
                 "Prefiro não dizer.\n" RESET);
      validar = 0;
    }
  } while (!validar);

  do {
    printf("Digite a idade do cliente: ");
    validar = scanf("%d", &c->idade);
    while ((ch = getchar()) != '\n' && ch != EOF)
      ;
    if (validar != 1 || c->idade <= 0) {
      printf(RED "\nErro: Idade inválida. Digite um número inteiro não "
                 "negativo.\n" RESET);
      validar = 0;
    }
  } while (validar != 1 || c->idade <= 0);
}

// Função para cadastrar as vendas
void cadastrar_venda(venda *v, FILE *file) {
  int validar;
  char ch;

  printf("Digite o número de itens da venda: ");
  validar = scanf("%d", &v->itens);
  while (validar != 1 || v->itens <= 0) {
    if (validar != 1) {
      while ((ch = getchar()) != '\n' && ch != EOF)
        ;
    }
    printf(
        RED
        "\nErro: Número de itens inválido. Digite um número positivo.\n" RESET);
    printf("Digite o número de itens da venda: ");
    validar = scanf("%d", &v->itens);
  }
  while ((ch = getchar()) != '\n' && ch != EOF)
    ;

  printf("Digite o número de sabores presentes na venda: ");
  validar = scanf("%d", &v->num_sabores);
  while (validar != 1 || v->num_sabores <= 0) {
    if (validar != 1) {
      while ((ch = getchar()) != '\n' && ch != EOF)
        ;
    }
    printf(RED "\nErro: Número de sabores inválido. Digite um número "
               "positivo.\n" RESET);
    printf("Digite o número de sabores presentes na venda: ");
    validar = scanf("%d", &v->num_sabores);
  }
  while ((ch = getchar()) != '\n' && ch != EOF)
    ;

  printf("Digite o valor total da compra: ");
  validar = scanf("%f", &v->valor_total);
  while (validar != 1 || v->valor_total <= 0) {
    if (validar != 1) {
      while ((ch = getchar()) != '\n' && ch != EOF)
        ;
    }
    printf(
        RED
        "\nErro: Valor da compra inválido. Digite um número positivo.\n" RESET);
    printf("Digite o valor total da compra: ");
    validar = scanf("%f", &v->valor_total);
  }
  while ((ch = getchar()) != '\n' && ch != EOF)
    ;
}

void pesquisar_vendas_de_cliente(venda *vendas, int num_vendas) {
  if (num_vendas == 0) {
    printf(RED "Nenhuma venda cadastrada.\n" RESET);
    return;
  }

  char nome_cliente[50];
  char continuar[10];

  do {
    int nome_valido = 1;
    do {
      nome_valido = 1;
      printf("Digite o nome do cliente a ser pesquisado: ");
      fgets(nome_cliente, sizeof(nome_cliente), stdin);
      nome_cliente[strcspn(nome_cliente, "\n")] = '\0';

      for (int i = 0; nome_cliente[i] != '\0'; i++) {
        if (!((nome_cliente[i] >= 'a' && nome_cliente[i] <= 'z') ||
              (nome_cliente[i] >= 'A' && nome_cliente[i] <= 'Z') ||
              nome_cliente[i] == ' ')) {
          nome_valido = 0;
          break;
        }
      }

      if (!nome_valido || strlen(nome_cliente) < 3) {
        printf(RED "\nErro: Nome inválido. Digite um nome com no mínimo 3 "
                   "letras e apenas caracteres alfabéticos.\n" RESET);
      }
    } while (!nome_valido || strlen(nome_cliente) < 3);

    int encontrado = 0;
    for (int i = 0; i < num_vendas; i++) {
      if (strcmp(vendas[i].cliente.nome, nome_cliente) == 0) {
        printf("\nVenda do cliente %s:\n", vendas[i].cliente.nome);
        printf("Sexo do cliente: %c\n", vendas[i].cliente.sexo);
        printf("Idade do cliente: %d\n", vendas[i].cliente.idade);
        printf("Número de itens: %d\n", vendas[i].itens);
        printf("Número de sabores: %d\n", vendas[i].num_sabores);
        printf("Valor total: %.2f\n", vendas[i].valor_total);
        printf("--------------------------\n");
        encontrado = 1;
      }
    }

    if (!encontrado) {
      printf(RED "\nCliente não encontrado.\n" RESET);
    }

    printf("Deseja pesquisar outro cliente? (S/N): ");
    fgets(continuar, sizeof(continuar), stdin);
    continuar[strcspn(continuar, "\n")] = '\0';
  } while (strcmp(continuar, "S") == 0 || strcmp(continuar, "s") == 0);
}

// Função para carregar todas as informações
void carregar_todas_vendas(venda *vendas, int qtd) {
  float valor_limite;
  int vendas_um_item = 0;
  int vendas_sabor_par = 0;
  int mulheres = 0;
  float valor_total_homens = 0.0;
  int qtd_total_itens = 0;
  int num_total_sabores = 0;
  float media_vendas_homens = 0.0;

  printf("\nDigite o valor máximo das compras a ser exibido: ");
  scanf("%f", &valor_limite);
  limpar_buffer();

  for (int i = 0; i < qtd; i++) {
    if (vendas[i].valor_total < valor_limite) {
      printf("\nVenda abaixo do valor máximo:\n\n");
      printf("Cliente: %s\n", vendas[i].cliente.nome);
      printf("Sexo do cliente: %c\n", vendas[i].cliente.sexo);
      printf("Idade do cliente: %d\n", vendas[i].cliente.idade);
      printf("Número de itens: %d\n", vendas[i].itens);
      printf("Número de sabores: %d\n", vendas[i].num_sabores);
      printf("Valor total: %.2f\n", vendas[i].valor_total);
      printf("--------------------------\n");
    }
  }

  for (int i = 0; i < qtd; i++) {
    if (vendas[i].itens == 1) {
      vendas_um_item++;
    }
    if (vendas[i].num_sabores % 2 == 0) {
      vendas_sabor_par++;
    }
    if (vendas[i].cliente.sexo == 'f' || vendas[i].cliente.sexo == 'F') {
      mulheres++;
    }
    if (vendas[i].cliente.sexo == 'm' || vendas[i].cliente.sexo == 'M') {
      valor_total_homens += vendas[i].valor_total;
    }
  }

  for (int i = 0; i < qtd; i++) {
    qtd_total_itens += vendas[i].itens;
    num_total_sabores += vendas[i].num_sabores;
  }

  int num_vendas_homens = 0;
  for (int i = 0; i < qtd; i++) {
    if (vendas[i].cliente.sexo == 'm' || vendas[i].cliente.sexo == 'M') {
      num_vendas_homens++;
    }
  }

  if (num_vendas_homens > 0) {
    media_vendas_homens = valor_total_homens / num_vendas_homens;
  }

  printf("\nQuantidade de vendas com somente 1 item: %d", vendas_um_item);
  printf("\nQuantidade de vendas com número par de sabores: %d",
         vendas_sabor_par);
  printf("\nQuantidade de vendas feitas por mulheres: %d", mulheres);
  printf("\nValor total das vendas feitas por homens: %.2f",
         valor_total_homens);
  printf("\nMédia de valor das compras feitas por homens: %.2f",
         media_vendas_homens);

  char cligigante[100] = "";
  int idx_cligigante = -1;

  for (int i = 0; i < qtd; i++) {
    if (strlen(vendas[i].cliente.nome) > strlen(cligigante)) {
      strcpy(cligigante, vendas[i].cliente.nome);
      idx_cligigante = i;
    }
  }

  if (idx_cligigante != -1) {
    printf("\nCliente com o nome mais longo: %s (Venda %d)\n", cligigante,
           idx_cligigante + 1);

    printf("\nVendas do cliente com o nome mais longo (%s):\n\n", cligigante);
    for (int i = 0; i < qtd; i++) {
      if (strcmp(vendas[i].cliente.nome, cligigante) == 0) {
        printf("Cliente: %s\n", vendas[i].cliente.nome);
        printf("Sexo do cliente: %c\n", vendas[i].cliente.sexo);
        printf("Idade do cliente: %d\n", vendas[i].cliente.idade);
        printf("Número de itens: %d\n", vendas[i].itens);
        printf("Número de sabores: %d\n", vendas[i].num_sabores);
        printf("Valor total: %.2f\n", vendas[i].valor_total);
        printf("--------------------------\n");
      }
    }
  }

  if (qtd > 0) {
    venda *cliente_mais_novo = &vendas[0];
    for (int i = 1; i < qtd; i++) {
      if (vendas[i].cliente.idade < cliente_mais_novo->cliente.idade) {
        cliente_mais_novo = &vendas[i];
      }
    }

    printf("\nVendas do cliente mais novo (%s, %d anos):\n\n",
           cliente_mais_novo->cliente.nome, cliente_mais_novo->cliente.idade);
    for (int i = 0; i < qtd; i++) {
      if (vendas[i].cliente.idade == cliente_mais_novo->cliente.idade &&
          strcmp(vendas[i].cliente.nome, cliente_mais_novo->cliente.nome) ==
              0 &&
          vendas[i].cliente.sexo == cliente_mais_novo->cliente.sexo) {

        printf("Cliente: %s\n", vendas[i].cliente.nome);
        printf("Sexo do cliente: %c\n", vendas[i].cliente.sexo);
        printf("Idade do cliente: %d\n", vendas[i].cliente.idade);
        printf("Número de itens: %d\n", vendas[i].itens);
        printf("Número de sabores: %d\n", vendas[i].num_sabores);
        printf("Valor total: %.2f\n", vendas[i].valor_total);
        printf("--------------------------\n");
      }
    }
  }

  char continuar[10];
  printf("Deseja retornar ao menu? (S/N): ");
  fgets(continuar, sizeof(continuar), stdin);
  continuar[strcspn(continuar, "\n")] = '\0';

  if (strcmp(continuar, "N") == 0 || strcmp(continuar, "n") == 0) {
    printf("Programa encerrando...\n");
    exit(0);
  }
}

int carregar_vendas_arquivo(venda **vendas) {
  FILE *file = fopen("registros.txt", "r");
  if (file == NULL) {
    printf(RED "Erro ao abrir o arquivo\n" RESET);
    return 0;
  }

  int num_vendas = 0;
  int capacidade = 10;
  *vendas = (venda *)malloc(capacidade * sizeof(venda));
  if (*vendas == NULL) {
    printf(RED "Erro ao alocar memória\n" RESET);
    fclose(file);
    return 0;
  }

  while (!feof(file)) {
    if (num_vendas >= capacidade) {
      capacidade *= 2;
      *vendas = (venda *)realloc(*vendas, capacidade * sizeof(venda));
      if (*vendas == NULL) {
        printf(RED "Erro ao alocar memória\n" RESET);
        fclose(file);
        return num_vendas;
      }
    }

    venda *v = &(*vendas)[num_vendas];
    fscanf(file, " Cliente: %[^\n]\n", v->cliente.nome);
    fscanf(file, " Sexo do cliente: %c\n", &v->cliente.sexo);
    fscanf(file, " Idade do cliente: %d\n", &v->cliente.idade);
    fscanf(file, " Número de itens: %d\n", &v->itens);
    fscanf(file, " Número de sabores: %d\n", &v->num_sabores);
    fscanf(file, " Valor total: %f\n", &v->valor_total);
    fscanf(file, "--------------------------\n");

    num_vendas++;
  }

  fclose(file);
  return num_vendas;
}
