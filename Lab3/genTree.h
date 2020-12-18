#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef struct t_node {
    int key;
    struct t_node* son;
    struct t_node* brother;
} Node, *Tree;

vector <char> strTovec(string number);
Tree initTree();
Tree createNode(int key, Node* son, Node* brother);
void addNode(int key, Tree &root, vector <char> path);
void findNode(int key, Tree root, vector <char> path, char add);
void printTree(Tree root, int level = 0);
