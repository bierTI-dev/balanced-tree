#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS

typedef char ru_t[8]; /*Define um RU com 7 posicoes*/

typedef struct Record record_t; /*Record = record_t*/
record_t* new_record(ru_t ru, char* name, char* email); /*Recebe ru, nome e e-mail*/

typedef struct Tree tree_t; /*Cria��o da �rvore*/
tree_t* new_tree(record_t* value);
tree_t* insert(tree_t* root, record_t* value); /*Insere o elemento em um arvore que ja existe*/
tree_t* balance(tree_t* root); /*Balanceia a arvore*/
record_t* find(tree_t* root, ru_t ru); /*Encontra o elemento na �rvore*/
int height(tree_t* root);
void drop_tree(tree_t* root);
void print_tree(tree_t* root, int level, char* extra);

void menu(tree_t* root);

int main(void) {
    tree_t* t = NULL; /*Inicializa com nulo*/

    /*Cria um novo elemento*/
    t = insert(t, new_record("3529473", "Victor Hugo Bier Silva", "victor@alunouninter.com"));//
    t = insert(t, new_record("0000501", "Fabricio", "fabricio@alunouninter.com"));//
    t = insert(t, new_record("0000502", "Joao", "joao@alunouninter.com"));//
    t = insert(t, new_record("0000503", "Mauricio", "mauricio@alunouninter.com"));//
    t = insert(t, new_record("0000504", "Vinicius", "vinicius@alunouninter.com"));//
    t = insert(t, new_record("0000505", "Juarez", "juarez@alunouninter.com"));//
    t = insert(t, new_record("0000506", "Isaias", "isaias@alunouninter.com"));//
    t = insert(t, new_record("0000507", "Cleber", "cleber@alunouninter.com"));//
    t = insert(t, new_record("0000508", "Carlos", "carlos@alunouninter.com"));//
    t = insert(t, new_record("0000509", "Marcus", "marcus@alunouninter.com"));//

    print_tree(t, 1, " ");
    menu(t);
    drop_tree(t); /*Libera a mem�ria da �rvore*/

    return 0; /*Retorna sucesso*/
}

struct Record { /*Constroi a estrutura "Record" e declara as variaveis*/
    ru_t ru;
    char name[51];
    char email[51];
};

record_t* new_record(ru_t ru, char* name, char* email) { /*Novo aluno*/
    record_t* r = calloc(sizeof(record_t), 1);

    if (r == NULL) {
        printf("n�o foi poss�vel alocar mem�ria para um novo registro\n");
        exit(1);
    }

    /*Copia at� encontrar um caractere nulo*/
    memcpy(r->ru, ru, sizeof(char) * 7);
    memcpy(r->name, name, sizeof(char) * 50);
    memcpy(r->email, email, sizeof(char) * 50);

    return r;
}

struct Tree { /*Estrutura recursiva*/
    ru_t ru;
    record_t* record;
    tree_t* left, * right;
};

/*aloca espa�o para uma nova �rvore, ent�o cria uma nova �rvore*/
tree_t* new_tree(record_t* record) {
    if (record == NULL) return NULL;

    tree_t* t = calloc(sizeof(tree_t), 1);

    if (t == NULL) {
        printf("n�o foi poss�vel alocar mem�ria para uma nova �rvore\n");
        exit(2);
    }

    t->record = record;
    memcpy(t->ru, record->ru, sizeof(char) * 7);
    return t;
}

/*Insere um elelmento na �rvore*/
tree_t* insert(tree_t* root, record_t* record) {
    if (root == NULL) return new_tree(record);

    int ord = strcmp(record->ru, root->ru);

    if (ord < 0) {
        root->left = insert(root->left, record);
    }
    else if (ord > 0) {
        root->right = insert(root->right, record);
    }

    return balance(root);
}

/*Balanceia a arvore*/
tree_t* balance(tree_t* root) {
    int l, r, diff;
    tree_t* target, * parent;

    l = height(root->left);
    r = height(root->right);
    diff = r - l;

    if (diff < -1) {
        parent = root;
        target = root->left;
        while (target->right != NULL) {
            parent = target;
            target = target->right;
        }
        target->right = root;
        if (parent != root) {
            target->left = root->left;
            parent->right = NULL;
        }
        root->left = NULL;
        target->left = balance(target->left);
        return target;
    }
    else if (diff > 1) {
        parent = root;
        target = root->right;
        while (target->left != NULL) {
            parent = target;
            target = target->left;
        }
        target->left = root;
        if (parent != root) {
            target->right = root->right;
            parent->left = NULL;
        }
        root->right = NULL;
        target->right = balance(target->right);
        return target;
    }
    else {
        return root;
    }
}

/*Localiza elementos na �rvore*/
record_t* find(tree_t* root, ru_t ru) {
    if (root == NULL) return NULL;

    int ord = strcmp(ru, root->ru);

    if (ord == 0)     return root->record;
    else if (ord < 0) return find(root->left, ru);
    else              return find(root->right, ru);
}

/*Tamanho da �rvore*/
int height(tree_t* root) {
    if (root == NULL) return 0;

    int l, r;
    l = height(root->left);
    r = height(root->right);

    return 1 + (l > r ? l : r); /*soma +1 do maior entre eles (l e r)*/
}

void drop_tree(tree_t* root) {
    if (root == NULL) return; /*Se for nullo, retorna*/

    drop_tree(root->left); /*Enquanto tiver algo na esquerda da �rvore, remove*/
    drop_tree(root->right); /*Enquanto tiver algo na direita da �rvore, remove*/
    free(root->record); /*Libera memoria do record*/
    free(root); /*Libera memora do root*/
}

/*Prints da �rvore*/
void print_tree(tree_t* root, int level, char* extra) {
    if (root == NULL) return;

    printf("%*s%s\n", level, extra, root->ru);
    print_tree(root->left, level + 2, "<");
    print_tree(root->right, level + 2, ">");
}

/*Menu do programa principal*/
void menu(tree_t* root) {
    char line[51];
    int op = 0;

    while (op != 4) {
        printf("Selecione uma op��o:\n");
        printf("1 - Buscar aluno\n");
        printf("2 - Inserir aluno\n");
        printf("3 - Imprimir �rvore\n");
        printf("4 - Encerrar\n");
        fgets(line, 51, stdin);

        if (sscanf_s(line, "%d", &op) != 1) continue;
        ru_t ru;
        record_t* r;

        switch (op) {
        case 1:
            printf("Digite o RU do aluno: ");
            fgets(line, 51, stdin);

            if (sscanf_s(line, "%[0-9]", ru, 8) != 1) {
                printf("RU inv�lido; precisa ser uma string de 7 d�gitos.\n\n");
                continue;
            }

            if ((r = find(root, ru)) != NULL) {
                printf("RU %s encontrado.\n", ru);
                printf("  Nome:  %s\n", r->name);
                printf("  Email: %s\n", r->email);
            }
            else {
                printf("RU %s n�o encontrado.\n", ru);
            }

            break;

        case 2:
            printf("Digite o RU do aluno: ");
            fgets(line, 51, stdin);

            if (sscanf_s(line, "%[0-9]", ru, 8) != 1) {
                printf("RU inv�lido; precisa ser uma string de 7 d�gitos.\n\n");
                continue;
            }

            char name[51], email[51];
            printf("Digite o nome do aluno: ");
            fgets(name, 51, stdin);
            *(strrchr(name, '\n')) = 0;
            printf("Digite o email do aluno: ");
            fgets(email, 51, stdin);
            *(strrchr(email, '\n')) = 0;

            root = insert(root, new_record(ru, name, email));
            printf("Aluno inserido com sucesso.\n");
            break;

        case 3:
            print_tree(root, 1, " ");
            break;

        case 4:
            printf("Encerrando programa...\n");
            break;

        default:
            printf("Op��o inv�lida.\n");
        }

        printf("\n");
    }
}
