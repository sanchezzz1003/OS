#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <pthread.h>

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD -12
#define SUCCESS 0
#define ERROR 1

using namespace std;

typedef struct t_node {
    int key;
    struct t_node* son;
    struct t_node* brother;
} Node, *Tree;

typedef struct someArgs {
	int key, counter, NUM_THREADS;
	Tree root;
	std::vector<char> path;
	char add;
	vector <pthread_t> threads;

} someArgs_t;

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
   // cout << endl << "vec: " << path.back() << " root: " << root << " key: " << key << endl;
    if (path.back() == 'r' || root == NULL)
      root = createNode(key, NULL, NULL);
    else if (path.back() == 's') {
        // если  к сыну
        path.pop_back();

        addNode(key, root->son, path);
    }
    else if (path.back() == 'b') {
        // если к брату
        path.pop_back();
        addNode(key, root->brother, path);
    }
}

void * findNode(void *args) {
	int status, status_addr;
	someArgs_t *arg = (someArgs_t*) args;
    arg->path.push_back(arg->add);
//    cout << arg->counter << endl;
    vector <char> copy;
    vector <pthread_t> threads = arg->threads;
//    cout << arg->key << " " << arg->root->key << endl;
    if (arg->root != NULL) {
        if (arg->key == arg->root->key){
          for (unsigned int i = 0; i < arg->path.size(); i++){
          	if (arg->path[i] == 'r') 
            	cout << arg->path[i];
            else cout << "->" << arg->path[i];
        	}
        	cout << endl;
        }
        else {
        	Tree buf = arg->root;
        	copy = arg->path;
        	if (arg->root->son != NULL){
        		arg->add = 's';
        		arg->root = arg->root->son;
	        	if (arg->counter < arg->NUM_THREADS) {
	        		arg->counter++;
	        		cout << arg << endl;
		            status = pthread_create(&threads[arg->counter], NULL, findNode, (void*) arg);
	//	            printf("create i = %d, thread %d\n", arg->counter, arg->threads[arg->counter]);
		            if (status != SUCCESS) {
		                printf("main error: can't create thread, status = %d\n", status);
		                exit(ERROR_CREATE_THREAD);
		            }
		            cout << "created\n";
		            status = pthread_join(threads[arg->counter], (void**)&status_addr);
	//            	printf("join i = %d, thread %d\n", arg->counter, threads[arg->counter]);
		            if (status != SUCCESS) {
		                printf("main error: can't join thread, status = %d\n", status);
		                exit(ERROR_JOIN_THREAD);
	            	}
	            	cout << "joined\n";
	           	}
	           	else findNode((void*) arg);
	        }
	        arg->path = copy;
	        arg->root = buf;
         //  	cout << "here4\n";
        //	cout << "here5\n";
        	if (arg->root->brother != NULL){
        		arg->add = 'b';
        		arg->root = arg->root->brother;
            	findNode((void*) arg);
            }
        }
    }
    return 0;
}

void printTree(Tree root, int level = 0) {
    if(level == 0)printf("-----------------\n");
    if (root != NULL) {
        printTree(root->son,level + 5);
        printf("%*d\n", level, root->key);
        printTree(root->brother,level);
    }
}

int main(int argc, char ** argv) {
	if (argc != 2){
		perror("args: size in bytes");
		exit(ERROR);
	}
	int NUM_THREADS = atoi(argv[1]);

	//clock_t start, stop;
	vector <pthread_t> threads(NUM_THREADS);
    string str;
    vector <char> v;
    int key, action;
    someArgs_t * const args = (someArgs_t*) malloc(sizeof(someArgs_t));

    NUM_THREADS--;
//    cout << "treads q: " << NUM_THREADS << " vector size : " << threads.size() << endl;
    Tree tree = initTree();
    do {
    	printf("Options\n");
		printf("1. Insert element\n");
		printf("2. Tree print\n");
		printf("3. Start search\n");
		printf("4. Help\n");
		printf("5. Exit\n");
		printf("Choose option: ");
		scanf("%d", &action);

		switch(action){
			case 1: {
				while (1) {
					cin >> str;
					if (str != "") {
						v = strTovec(str);
					}
					else break;
					if (str == "stop") break;				
					v.push_back('+');
					reverse(v.begin(), v.end());
					cin >> key;
					addNode(key, tree, v);
				}
				break;
			}
			case 2: {
				printTree(tree);
				break;
			}
			case 3: {
				cout << "Enter key: ";
				cin >> key;
		//		start = clock();
				while (v.size() > 0) v.pop_back();
				args->key = key;
				args->root = tree;
				args->add = 'r';
				args->counter = 0;
				args->NUM_THREADS = NUM_THREADS;
				args->threads = threads;
				findNode((void*) args);
				free(args);
				args->path.clear();
				//stop = clock();
			//	cout << "vector size: " << threads.size() << " done in : " << ((float)(stop - start)) << " clocks" << endl;
				break;
			}
			case 4: {
				printf("enter tree like that: r 1 s 2 ss 3 sb 4...\n");
			}
			case 5: break;
			default: {
				printf("Error. Wrong option\n");
				break;
			}
		}
    }
    while (action != 5);
    return 0;
}