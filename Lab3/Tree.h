#include <iostream>

using namespace std;

struct Tnode {
	int key;
	struct Tnode *son;
	struct Tnode *brother;
};
typedef struct Tnode Node;

Node *create_tree(int v){
	Node *Tree = (Node *) malloc(sizeof(Node));
	Tree->key = v;
	Tree->son = NULL;
	Tree->brother = NULL;
	return Tree;
}

Node add_node(Node **tree, const char *a){
	Node* t = *tree;
	int value;
	scanf("%d", &value);
	int i = 0;
	while (a[++i] != '\0'){
		if (a[i] == 'r') {
			*tree = create_tree(value); // создаем корень
			t = *tree;
			return *tree;
		}
		if (a[i] == 's') {
          if (t = to_son(t)) // функция, которая возвращает указатель на сына
            continue;
          return NULL; //иначе NULL
        }
        if (a[i] == 'b') {
          if (t = to_brother(t)) //возвращает указатель на брата t 
            continue;
          return NULL;
        }
	} 
	if (t->son !=NULL) {
		t = last_son(t);
		t->brother = create_tree(value);
		return t->brother;
	}
	else {
		t->son = create_tree(value);
		return t->son;
	}
}

Node last_son(Node **tree) {
	Node* t = *tree;
	while (t->brother != NULL)
		t = to_brother(t);
	return t;
}

Node to_brother(Node **tree){
	Node * t = *tree;
	if (t->brother != NULL){
		t = t->brother;
		return t;
	}
	else return 0;
}

Node to_son(Node **tree){
	Node * t = *tree;
	if (t->son != NULL){
		t = t->son;
		return t;
	}
	else return 0;
}