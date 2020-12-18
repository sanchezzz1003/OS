#include "genTree.h"

vector <char> strTovec(string number) {
  vector <char> v;
  int flag = 0;
  for (long unsigned int i = 0; i < number.length(); i++) {
      if (number[i] != '0' && flag == 0)
          flag = 1;
          if (number[i] == '0' && flag == 1)
            v.push_back(number[i]);
          else if (number[i] != '0')
            v.push_back(number[i]);
    }
    if (v.size() == 0) {
        v.push_back('0');
    }
    return v;
}

Tree initTree() { return NULL; }

Tree createNode(int key, Node* son, Node* brother) {
    Tree root;
    root = (Node*)malloc(sizeof(Node));
    root->key = key;
    root->son = son;
    root->brother = brother;
    return root;
}

void addNode(int key, Tree &root, vector <char> path) {
  //  cout << endl << "vec: " << path.back() << " root: " << root << " key: " << key << endl;
    if (path.back() == 'r' || root == NULL)
      root = createNode(key, NULL, NULL);
    else if (path.back() == 's') {
        // если  к сыну
        cout << "to son" << endl;
        path.pop_back();

        addNode(key, root->son, path);
    }
    else if (path.back() == 'b') {
        // если к брату
        path.pop_back();
        addNode(key, root->brother, path);
    }
}

void findNode(int key, Tree root, vector <char> path, char add) {
    path.push_back(add);
    while (root != NULL) {
        if (key == root->key){
          for (int i = 0; i < path.size(); ++i)
            cout << path[i] << " ";
          cout << endl;
        }
        else {
            findNode(key, root->son, path, 's');
            findNode(key, root->brother, path, 'b');
        }
    }
}
/*
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
*/
void printTree(Tree root, int level = 0) {
    if(level == 0)printf("-----------------\n");
    if (root != NULL) {
        printTree(root->son,level + 5);
        printf("%*d %p\n", level, root->key, root);
        printTree(root->brother,level);
    }
}
