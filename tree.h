#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// Ast stands for Abstract Syntax Tree

typedef struct treeNode {
    char* value;
    struct treeNode* left;
    struct treeNode* right;
} ast;

typedef struct listAst {
    ast* tree;
    struct listAst* next;
} l_ast;

typedef struct listAsig {
    char name[220];
    char v[220];
    int i;
    float f;
    struct listAsig* next;
} asigSymbol;


// AllEqual
typedef struct expresionList {
  ast* item;
  struct expresionList* prevItem;
  struct expresionList* nextItem;
} el;

typedef struct allEqualList {
  el* list;
  struct allEqualList* nextList;
  struct allEqualList* prevList;
} ael;


void printAndSaveAST();
void printAST();
ast* newNode();
ast* newLeaf();
ast* incrustarArbol();

// AllEqual
void insertarNodoEnListaAllEqual(ast* node);
el* crearNuevaListaExpresiones();
ast* crearNodosCondicionAllEqual();
void comparar(ast* valu1, ast* value2);

FILE *file;

l_ast* listaAsigM = NULL;
asigSymbol* reverseList();
asigSymbol* asigMList;
void clearAsigMList();
void printAsigM();
int cantAvanzar();
void deleteLastNode();
ast* finishAsig();
void printM();
l_ast* insertMult();

// AllEqual
ael* allEqual = NULL;
el* listaAllEqual = NULL;
ast* allEqualCondition = NULL;

int contador = 1;

ael* crearAllEqual() {
  ael* newAllEqual = (ael*) malloc(sizeof(ael));
  newAllEqual->list = NULL;
  newAllEqual->nextList = NULL;
  newAllEqual->prevList = NULL;
  return newAllEqual;
}

void cerrarListaAllEqual() {
  if(allEqual == NULL) {
    allEqual = crearAllEqual();
    allEqual->nextList = crearAllEqual();
  }

  if(allEqual->list != NULL) {
    ael* aux = allEqual;
    allEqual = allEqual->nextList;
    allEqual->prevList = aux;
    allEqual->list = listaAllEqual;
    allEqual->nextList = crearAllEqual();
  } else {
    allEqual->list = listaAllEqual;
    listaAllEqual = NULL;
  }
}

void comparar(ast* value1, ast* value2) {
  ast* comparison = newNode("==", value1, value2);
  if(allEqualCondition == NULL) {
    allEqualCondition = comparison;
  } else {
    ast* andNode = newNode("AND", allEqualCondition, comparison);
    allEqualCondition = andNode;
  }
}

ast* crearNodosCondicionAllEqual() {
  int cont = 0;

  while(allEqual) {
    ael* auxAllEqual = allEqual->prevList;
    int listPosition = 0;

    cont ++;

    while(allEqual->list) {
      int auxListPosition = 0;

      while(auxListPosition < listPosition) {
        if(auxAllEqual->list->prevItem) {
          auxAllEqual->list = auxAllEqual->list->prevItem;
        }
        auxListPosition++;
      }

      while(auxAllEqual) {
          comparar(allEqual->list->item, auxAllEqual->list->item);
          auxAllEqual = auxAllEqual->prevList;
      }

      allEqual->list = allEqual->list->prevItem;
      auxAllEqual = allEqual->prevList;

      listPosition++;
    }

    printf("\n");
    allEqual = allEqual->prevList;
  }
  return allEqualCondition;
}

el* crearNuevaListaExpresiones() {
  el* nuevaLista = (el*) malloc(sizeof(el));
  nuevaLista->item = NULL;
  nuevaLista->nextItem = NULL;
  return nuevaLista;
};

void insertarNodoEnListaAllEqual (ast* node) {
  if(listaAllEqual == NULL) {
    listaAllEqual = crearNuevaListaExpresiones();
    listaAllEqual->item = node;
    listaAllEqual->nextItem = crearNuevaListaExpresiones();
    return;
  }

  if(listaAllEqual->item != NULL) {
    el* aux = listaAllEqual;
    listaAllEqual = listaAllEqual->nextItem;
    listaAllEqual->prevItem = aux;
    listaAllEqual->item = node;
    listaAllEqual->nextItem = crearNuevaListaExpresiones();
  }
}

char* searchValue(asigSymbol* a) {
    char s[220];
    int len;
    char* aux;
    strcpy(s,"");
    if (strcmp(a->v,"") == 0) {
        if (a->i == 0) {
            if (a->f != 0.00) {
                gcvt (a->f, 7, s);
                len = strlen(s);
                aux = (char*) malloc(len+1);
                strcpy(aux,s);
                s[0] = '_' ;
                concatenate(s, aux);
            }
        } else {
            itoa(a->i,s,10);
            len = strlen(s);
            aux = (char*) malloc(len+1);
            strcpy(aux,s);
            s[0] = '_' ;
            concatenate(s, aux);
        }
    } else {
        strcpy(s,a->v);
    }
    return strdup(s);
}

l_ast* insertMult(ast* t) {
    l_ast* node = (l_ast*) malloc(sizeof(l_ast));
    node->tree = t;
    node->next = listaAsigM;
    listaAsigM = node;
}

ast* finishAsig() {
    char* phraseAsig = "Asig_M";
    l_ast* current = listaAsigM;
    ast* node = current->tree;
    if(current->next != NULL) {
        current = current->next;
    } else {
        return node;
    }
    while (current != NULL) {
        node = newNode(phraseAsig, current->tree, node);
        current = current->next;
    }
    return node;
}

void printM() {
    l_ast* current = listaAsigM;
    while (current != NULL) {
        printf("\n-----\n%s - %s - %s\n-----\n", current->tree->value, current->tree->left->value, current->tree->right->value);
        current = current->next;
    }
}

int length() {
    int cant = 0;
    asigSymbol* aux = asigMList;
    while (aux != NULL) {
        cant++;
        aux = aux->next;
    }
    return cant;
}

ast* incrustarArbol() {
    char* symbolAsig = ":=";
    ast* asigT;
    int i;
    asigSymbol* aux = reverseList();
    i = length();
    for ( i; i>0; i--){
        insertMult(newNode(symbolAsig, newLeaf(aux->name), newLeaf(searchValue(aux))));
        deleteLastNode();
        aux = reverseList();
    }
    return finishAsig();
}

void deleteLastNode() {
    asigSymbol* aux = asigMList;
    asigSymbol* ant = asigMList;
    while (aux->next != NULL) {
        ant = aux;
        aux = aux->next;
    }
    ant->next = NULL;
    free(aux);
}

asigSymbol* reverseList() {
    asigSymbol* aux = asigMList;
    asigSymbol* ant = asigMList;
    while (aux != NULL) {
        ant = aux;
        aux = aux->next;
    }
    return ant;
}

int cantAvanzar() {
    int cant = 0;
    asigSymbol* aux = asigMList;
    while (aux != NULL) {
        if (strcmp(aux->v,"") == 0) {
            if (aux->i == 0) {
                if (aux->f == 0.00) {
                    cant++;
                }
            }
        }
        aux = aux->next;
    }
    return cant-1;
}

asigSymbol* insertAsigM(char* value, int choice, float f, int i, int t) {
    asigSymbol* node = (asigSymbol*) malloc(sizeof(asigSymbol));
    asigSymbol* aux = asigMList;
    int avanzar;
    if (choice == 1) {
        int len = strlen(value);
        char* valueToInsert = (char*) malloc(len+1);
        strcpy(valueToInsert, value);
        strcpy(node->name, valueToInsert);
        node->next = asigMList;
        asigMList = node;
    } else if (choice == 2) {
        avanzar = cantAvanzar();
        while (avanzar != 0) {
            aux = aux->next;
            avanzar--;
        }
        node = aux;
        if (t == 1){ //string
            int len;
            if (value[0] == '"') {
                removeChar(value, '"');
                len = strlen(value);
                char* aux = (char*) malloc(len+1);
                strcpy(aux,value);
                value[0] = '_' ;
                concatenate(value, aux);
            }
            len = strlen(value);
            char* valueToInsert = (char*) malloc(len+1);
            strcpy(valueToInsert, value);
            strcpy(node->v, valueToInsert);
        } else if (t == 2){// entero
            node->i = i;
        } else if (t == 3){// float
            node->f = f;
        }
    }
    return node;
}

void clearAsigMList() {
    asigMList = NULL;
}

void printAsigM(asigSymbol* a) {
    asigSymbol* current = a;
    printf("\n LISTA ASIG MULT \n");
    while(current != NULL){
        printf("%s - %s - %f - %d\n", current->name, current->v, current->f, current->i);
        current = current->next;
    }
}

ast* newNode(char* operation, ast* leftNode, ast* rightNode) {
    ast* node = (ast*) malloc(sizeof(ast));
    node->value = operation;
    node->left = leftNode;
    node->right = rightNode;
    return node;
}

ast* newLeaf(char* value) {
    ast* node = (ast*) malloc(sizeof(ast));

    node->value = strdup(value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

void printAST(ast* tree) {
     if (tree == NULL)
          return;

     /* first recur on left child */
     printAST(tree->left);

     /* then print the data of node */
     fprintf(file, "%s ", tree->value);

     /* now recur on right child */
     printAST(tree->right);
}

void printAndSaveAST(ast* tree) {
    ast* copy = tree;
    file = fopen("intermedia.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    printAST(copy);
    fclose(file);
}
