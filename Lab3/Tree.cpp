#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

typedef struct t_node {
    int key;
    struct t_node* left;
    struct t_node* right;
} Node, *Tree;

Tree initTree() { return NULL; }

Tree createNode(int key, Node* left, Node* right) {
    Tree root;
    root = (Node*)malloc(sizeof(Node));
    root->key = key;
    root->left = left;
    root->right = right;
    return root;
}

void addNode(int key, Tree &root) {
	cout << endl << " root: " << root << " key: " << key << endl;
    if (root == NULL) root = createNode(key, NULL, NULL);
    else if (key > root->key) {
        // если в правое поддерево
        addNode(key, root->right);
    }
    else if (key < root->key) {
        // если в левое поддерево
        addNode(key, root->left);
    }
}

Tree findNode(int key, Tree root) {
    int find = 0;
    while ((root != NULL) && !(find)) {
        if (key == root->key) find = 1;
        else {
            if (key < root->key) root = findNode(key, root->left);
            else root = findNode(key, root->right);
        }
    }
    return root;
}

Tree minNode(Tree root) {
    if (root->left == NULL) return root;
    return minNode(root->left);
}

Tree maxNode(Tree root) {
    if (root->right == NULL) return root;
    return maxNode(root->right);
}

Tree delNode(int key, Tree root) {
    if (root == NULL) return root;

    if (key < root->key)
        root->left = delNode(key, root->left);
    else
        if (key > root->key) root->right = delNode(key, root->right);
        else
            if (root->left != NULL && root->right != NULL) {
                root->key = minNode(root->right)->key;
                root->right = delNode(root->right->key, root->right);
            }
            else
                if (root->left != NULL)
                    root = root->left;
                else
                    root = root->right;
    return root;
}

void printTree(Tree root, int level = 0) {
    if(level == 0)printf("-----------------\n");
    if (root != NULL) {
        printTree(root->right,level + 5);
        printf("%*d %p\n", level, root->key, root);
        printTree(root->left,level + 5);
    }
}


int main(/* int argc, char *argv[] */) {


    Tree tree = initTree();
    addNode(4, tree);
    printTree(tree);

    addNode(5,tree);
    printTree(tree);
    
    addNode(1, tree);
    printTree(tree);
    
    addNode(2, tree);
    printTree(tree);
    
    addNode(7, tree);
    printTree(tree);

    return 0;
}