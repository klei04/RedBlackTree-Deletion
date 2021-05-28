//This program is a red-black tree that allows for
//insertion (through the console or a file),
//display, search, and removal!
//Author: Kevin Lei
//Date: 5/27/2021

//Used the following sites for insertion algorithm help:
//https://www.geeksforgeeks.org/red-black-tree-set-2-insert/
//https://www.programiz.com/dsa/insertion-in-a-red-black-tree
//https://iq.opengenus.org/red-black-tree-insertion/
//https://www.cs.auckland.ac.nz/software/AlgAnim/red_black.html

//Used the following sites for removal algorithm help:
//http://software.ucv.ro/~mburicea/lab8ASD.pdf
//https://www.youtube.com/watch?v=eO3GzpCCUSg&t=74s

#include <iostream>
#include <cstring>
#include <fstream>
#include <stdlib.h>

using namespace std;

//Constants for colors of the nodes
const int RED = 0;
const int BLACK = 1;

//Node struct
struct Node {
  int data;
  Node* left = NULL;
  Node* right = NULL;
  Node* parent = NULL;
  int color = RED;
};

//Function prototypes
void ADD(Node* & root);
void READ(Node* & root);
void insert(Node* & root, int newData);
void insertFix(Node* & root, Node* & passedNode);
Node* getUncle(Node* nephew);
void leftRotate(Node* & root, Node* & passedNode);
void rightRotate(Node* & root, Node* & passedNode);
void PRINT(Node* root, int depth);
Node* SEARCH(Node* root, int data);
Node* findMin(Node* root);
void REMOVE(Node* & root, int data);
Node* getSibling(Node* sibling);
void removeFix (Node* & root, Node* & passedx);
Node* removeNULL(Node* & root);
void CLEAR(Node* & root);

int main() {
  Node* root = NULL; //Creating the root
  cout << "Welcome to Red-Black Tree" << endl;
  char input[10];

  //While loop will run as long as user doesn't type QUIT
  while(strcmp(input, "QUIT") != 0) {
    cout << "Type ADD, READ, PRINT, SEARCH, REMOVE, CLEAR, or QUIT" << endl;
    cin >> input;

    if (strcmp(input, "ADD") == 0) { //Console input
      ADD(root);
    } else if (strcmp(input, "READ") == 0) { //File input
      READ(root);
    } else if (strcmp(input, "PRINT") == 0) { //Print tree
      PRINT(root, 0);
    } else if (strcmp(input, "SEARCH") == 0) { //Search tree
      cout << "Type what you would like to search for" << endl;
      int data;
      cin >> data;

      if (SEARCH(root, data) != NULL) {
	cout << data << " was found" << endl;
      } else {
	cout << data << " was not found" << endl;
      }
    } else if (strcmp(input, "REMOVE") == 0) { //Remove from tree
      cout << "Type what you would like to remove" << endl;
      int data;
      cin >> data;

      if (SEARCH(root, data) == NULL) { //First checking whether or not the data is in the tree
	cout << data << " was not found" << endl;
      } else {
	REMOVE(root, data);
      }
    } else if (strcmp(input, "CLEAR") == 0) { //Clear entire tree
      CLEAR(root);
    } else if (strcmp(input, "QUIT") != 0) { //Invalid command
      cout << "Invalid command" << endl;
    }

    cout << endl;
  }

  cout << "Goodbye" << endl;
  return 0;
}

void ADD(Node* & root) { //Inputs numbers through the console
  char data[500];
  cout << "Type the numbers to be added to the tree" << endl;
  cin.get();
  cin.get(data, 500);
  cin.get();

  char* ptr;
  ptr = strtok(data, " ");
  while (ptr != NULL) {
    insert(root, atoi(ptr));
    ptr = strtok(NULL, " ");
  }
}

void READ(Node* & root) { //Inputs numbers through a file
  char data[500];
  char fileName[50];
  cout << "Type the name of the file" << endl;
  cin >> fileName;

  ifstream file;
  srand(time(0));
  file.open(fileName);
  if (!file) {
    cout << fileName << " could not be accessed" << endl;
    exit(1);
  } else {
    while (file >> data) {
      insert(root, atoi(data));
    }
  }
}

void insert(Node* & root, int newData) { //Inserts a number into the tree
  Node* current = root;
  Node* parent = NULL;

  while (current != NULL) {
    parent = current;
    if (newData < current->data) { //go left
      current = current->left;
    } else { //go right
      current = current->right;
    }
  }

  Node* newNode = new Node;
  newNode->data = newData;
  newNode->parent = parent;

  if (parent == NULL) { //if number was inserted at root
    root = newNode;
  } else if (newNode->data < parent->data) {
    parent->left = newNode;
  } else {
    parent->right = newNode;
  }

  insertFix(root, newNode);
  
}

void insertFix(Node* & root, Node* & passedNode) { //Fixes the red-black tree after insertion
  Node* newNode = passedNode;
  while (newNode->parent != NULL && newNode->parent->color != BLACK) { //Checks for Case 1 or 2: Insert at root or parent is black
    if (newNode->parent == newNode->parent->parent->left) {
      if (getUncle(newNode) != NULL && getUncle(newNode)->color == RED) { //Case 3: Parent and uncle are red
	//Make parent and uncle black, make grandparent red. Check grandparent.
	newNode->parent->color = BLACK;
	getUncle(newNode)->color = BLACK;
	newNode->parent->parent->color = RED;
	newNode = newNode->parent->parent;
      } else {
	if (newNode == newNode->parent->right) { //Case 4a: Uncle is black, parent is a left node, newNode is a right node
	  //Perform rotation through parent, call Case 5a
	  newNode = newNode->parent;
	  leftRotate(root, newNode);
	}
	//Case 5a: Uncle is black, parent is a left node, newNode is a left node
	//Switch parent and grandparent colors, perform rotation through grandparent
        newNode->parent->color = BLACK; 
	newNode->parent->parent->color = RED;
	rightRotate(root, newNode->parent->parent);
      }
    } else {
      if (getUncle(newNode) != NULL && getUncle(newNode)->color == RED) {  //Case 3: Parent and uncle are red
	//Make parent and uncle black, maje grandparent red. Check grandparent.
	newNode->parent->color = BLACK;
	getUncle(newNode)->color = BLACK;
	newNode->parent->parent->color = RED;
	newNode = newNode->parent->parent;
      } else {
	if (newNode == newNode->parent->left) {  //Case 4b: Uncle is black, parent is a right node, newNode is a left node
	  //Perform rotation through parent, call Case 5b
	  newNode = newNode->parent;
	  rightRotate(root, newNode);
	}
	//Case 5b: Uncle is black, parent is a right node, newNode is a right node
	//Switch parent and grandparent colors, perform rotation through grandparent
	newNode->parent->color = BLACK;
	newNode->parent->parent->color = RED;
	leftRotate(root, newNode->parent->parent);
      }
    }
    root->color = BLACK;
  }
  root->color = BLACK; //Case 1: Insert at root  
}

Node* getUncle(Node* nephew) { //Function that returns the uncle (left or right) of a node
  if (nephew->parent == nephew->parent->parent->left) {
    return nephew->parent->parent->right;
  } else {
    return nephew->parent->parent->left;
  }
}

void leftRotate(Node* & root, Node* & passedNode) { //Function that performs a left rotation
  Node* parentNode = passedNode;
  Node* childNode = parentNode->right;
  parentNode->right = childNode->left;
  if (childNode->left != NULL) {
    childNode->left->parent = parentNode;
  }
  childNode->parent = parentNode->parent;
  if (parentNode->parent == NULL) {
    root = childNode;
  } else {
    if (parentNode == parentNode->parent->left) {
      parentNode->parent->left = childNode;
    } else {
      parentNode->parent->right = childNode;
    }
  }
  childNode->left = parentNode;
  parentNode->parent = childNode;
}

void rightRotate(Node* & root, Node* & passedNode) { //Function that performs a right rotation
  Node* parentNode = passedNode;
  Node* childNode = parentNode->left;
  parentNode->left = childNode->right;
  if (childNode->right != NULL) {
    childNode->right->parent = parentNode;
  }
  childNode->parent = parentNode->parent;
  if (parentNode->parent == NULL) {
    root = childNode;
  } else {
    if (parentNode == parentNode->parent->left) {
      parentNode->parent->left = childNode;
    } else {
      parentNode->parent->right = childNode;
    }
  }
  childNode->right = parentNode;
  parentNode->parent = childNode;
}

void PRINT(Node* root, int depth) { //Displays the tree nicely
  if (root != NULL) {
    if (root->right != NULL) { //Displays right subtree
      PRINT(root->right, depth + 1);
    }
    
    for (int i = 0; i < depth; i++) {
      cout << "             ";
    }

    //Prints node's info (color, data, parent)
    if (root->color == BLACK) { 
      cout << root->data << "(p: ";
      if (root->parent == NULL) {
	cout << "NULL)" << endl;
      } else {
	cout << root->parent->data << ")" << endl;
      }
    } else {
      cout << "\033[31m" << root->data << "(p: ";
      if (root->parent == NULL) {
	cout << "NULL)" << "\033[0m" << endl;
      } else {
	cout << root->parent->data << ")" << "\033[0m" << endl;
      }
    }

    if (root->left != NULL) { //Displays left subtree
      PRINT(root->left, depth + 1);
    }
  }
  return;
}

//Function searches the binary search tree for a specific integer
//and returns the pointer if it is found or NULL if it is not found
Node* SEARCH(Node* root, int data) {
  Node* current = root;
  while (current != NULL) {
    if (data < current->data) {
      current = current->left;
    } else if (data > current->data) {
      current = current->right;
    } else {
      return current;
    }
  }
  return NULL;
}

//Function removes a specific integer from the tree
void REMOVE(Node* & root, int data) {
  Node* toDelete = SEARCH(root, data);

  Node* replacement = NULL;
  Node* x = NULL;

  if (toDelete->left == NULL || toDelete->right == NULL) { //If node to be deleted has 1 or 2 NULL children
    replacement = toDelete;
  } else  { //If node to be deleted has 0 NULL children
    //Finding next largest node to be the replacement
    replacement = findMin(toDelete->right);
  }

  //Setting x to be either replacement's sole child or a new "NULL" node
  if (replacement->left != NULL) {
    x = replacement->left;
  } else if (replacement->right != NULL) {
    x = replacement->right;
  } else {
    x = new Node;
    x->data = 0;
    x->color = BLACK;
  }

  //x's parent is now the node that was previously replacement's parent
  x->parent = replacement->parent;

  if (replacement->parent == NULL) { //If trying to delete the root
    root = x;
  } else if (replacement == replacement->parent->left) {
    replacement->parent->left = x;
  } else {
    replacement->parent->right = x;
  }

  if (replacement != toDelete) {
    toDelete->data = replacement->data;
  }

  if (replacement->color == BLACK) { //Fix if the replacement is black
    removeFix(root, x);
  }

  root = removeNULL(root); //Remove added "NULL" node
  delete replacement;

  

  /*if (toDelete->left == NULL && toDelete->right == NULL) { //If node to be deleted has 2 NULL children
    //Set replacement and x to the right NULL child
    //replacement = toDelete->right;
    //x = toDelete->right;

    replacement = new Node;
    replacement->data = 0;
    replacement->color = BLACK;
    replacement->parent = toDelete->parent;
    x = replacement;

    if (toDelete != root) { //Checking whether the node to be deleted is the root
      if (toDelete->parent->left == toDelete) { //If node to be deleted is a left child
	toDelete->parent->left = replacement;
      } else { //If node to be deleted is a right child
	toDelete->parent->right = replacement;
      }
    } else {
      root = replacement;
    }
  } else if (toDelete->left != NULL && toDelete->right == NULL) { //If node to be deleted only has a left child
    //Set replacement and x to the left child
    replacement = toDelete->left;
    x = toDelete->left;

    if (toDelete != root) { //Checking whether the node to be deleted is the root
      if (toDelete->parent->left == toDelete) { //If node to be deleted is a left child
	toDelete->parent->left = replacement;
	replacement->parent = toDelete->parent;
      } else { //If node to be deleted is a right child
	toDelete->parent->right = replacement;
	replacement->parent = toDelete->parent;
      }
    } else {
      root = replacement;
      replacement->parent = NULL;
    }
  } else if (toDelete->right != NULL && toDelete->left == NULL) { //If node to be deleted only has a right child
    //Set replacement and x to the right child
    replacement = toDelete->right;
    x = toDelete->right;

    if (toDelete != root) { //Checking whether the node to be deleted is the root
      if (toDelete->parent->left == toDelete) { //If node to be deleted is a left child
	toDelete->parent->left = replacement;
	replacement->parent = toDelete->parent;
      } else { //If node to be deleted is a right child
	toDelete->parent->right = replacement;
	replacement->parent = toDelete->parent;
      }
    } else {
      root = replacement;
      replacement->parent = NULL;
    }
  } else if (toDelete->left != NULL && toDelete->right != NULL) { //If node to be deleted has 2 children
    //Set replacement to the next largest node
    //Set x to the replacement's right child before the replacement is spliced out
    replacement = findMin(toDelete->right);
    x = replacement->right;

    if (x == NULL) {
      x = new Node;
      x->data = 0;
      x->color = BLACK;
      x->parent = replacement;
      
    }

    if (toDelete != root) { //Checking whether the node to be deleted is the root
      if (toDelete->parent->left == toDelete) { //If node to be deleted is a left child
	if (replacement == toDelete->right) { //Checking whether the replacement is the child of the node to be deleted
	  toDelete->parent->left = replacement;
	  replacement->parent = toDelete->parent;
	  replacement->left = toDelete->left;
	  toDelete->left->parent = replacement;
	} else {	  
	  toDelete->parent->left = replacement;
	  replacement->parent->left = x;
	  x->parent = replacement->parent;
	  replacement->parent = toDelete->parent;
	  replacement->left = toDelete->left;
	  replacement->right = toDelete->right;
	  toDelete->left->parent = replacement;
	  toDelete->right->parent = replacement;
	}
      } else { //If node to be deleted is a right child
	if (replacement == toDelete->right) {  //Checking whether the replacement is the child of the node to be deleted
	  toDelete->parent->right = replacement;
	  replacement->parent = toDelete->parent;
	  replacement->left = toDelete->left;
	  toDelete->left->parent = replacement;
	} else {
	  toDelete->parent->right = replacement;
	  replacement->parent->left = x;
	  x->parent = replacement->parent;
	  replacement->parent = toDelete->parent;
	  replacement->left = toDelete->left;
	  replacement->right = toDelete->right;
	  toDelete->left->parent = replacement;
	  toDelete->right->parent = replacement;
	}
      }
    } else {
      if (replacement == toDelete->right) {  //Checking whether the replacement is the child of the node to be deleted
	root = replacement;
	replacement->parent = NULL;
	replacement->left = toDelete->left;
	toDelete->left->parent = replacement;
      } else {
	root = replacement;
	replacement->parent->left = x;
	x->parent = replacement->parent;
	replacement->parent = NULL;
	replacement->left = toDelete->left;
	replacement->right = toDelete->right;
	toDelete->left->parent = replacement;
	toDelete->right->parent = replacement;
      }
    }
  }

  //cout << replacement->color << endl;
  cout << replacement->data;
  cout << x->data;
  
  //If the node to be deleted is red and its replacement is red or NULL,
  //we are done
  if (toDelete->color == RED && (replacement->data == 0 || replacement->color == RED)) {
    delete toDelete;
    //return;
  }

  
  //If the node to be deleted is red and its replacement is black,
  //color the replacement red and proceed to removeFix
  else if (toDelete->color == RED && replacement->color == BLACK) {
    delete toDelete;
    replacement->color = RED;
    removeFix(root, x);
    root = removeNULL(root);
    //return;
  }

  //If the node to be deleted is black and its replacement is red,
  //color the replacement black and we are done
  else if (toDelete->color == BLACK && replacement->color == RED) {
    delete toDelete;
    replacement->color = BLACK;
    //return;
  }
  

  //If the node to be deleted is black and its replacement is NULL or black,
  //proceed to removeFix
  else if (toDelete->color == BLACK && (replacement->data = 0 || replacement->color == BLACK)) {
    delete toDelete;
    removeFix(root, x);
    root = removeNULL(root);
    //return;
  }
  
    cout << replacement->data << endl;

    cout << x->data << endl;*/


}

void removeFix (Node* & root, Node* & passedx) {
  Node* x = passedx;
  Node* w = getSibling(x);

  if (x->color == RED) { //Case 0: x is red
    //Color x black and we are done
    x->color = BLACK;
    return;
  }

  while (x != root && x->color == BLACK) {
    if (x == x->parent->left) { //If x is a left child
      if (w->color == RED) { //Case 1: x is black and its sibling w is red
	//Color w black
	w->color = BLACK;
	//Color x's parent red
	x->parent->color = RED;
	//Left rotate x's parent
	leftRotate(root, x->parent);
	//Set w to x's parent's right
	w = x->parent->right;
	//Proceed to case 2, 3, or 4
      }

      if ((w->left == NULL || w->left->color == BLACK)
	  && (w->right == NULL || w->right->color == BLACK)) { //Case 2: x is black, its sibling w is black, and both of w's children are black
	//Color w red
	w->color = RED;
	//Set x to x's parent
	x = x->parent;
	//If new x is red, color x black (exit while loop) and we are done
	//If new x is black, go to case 1, 2, 3, or 4 (stay in while loop)
      } else if (w->right == NULL || w->right->color == BLACK) { //Case 3: x is black, sibling w is black, w's left child is red (implied), w's right child is black
	//Color w's left child black
	w->left->color = BLACK;
	//Color w red
	w->color = RED;
	//Right rotate w
	rightRotate(root, w);
	//Set w to x's parent's right
	w = x->parent->right;
	//Now stay in while loop to proceed to case 4
      } else  { //Case 4: x is black, sibling w is black, w's right child is red
	//Color w the same color as x's parent
	w->color = x->parent->color;
	//Color x's parent black
	x->parent->color = BLACK;
	//Color w's right child black
	if (w->right != NULL) {
	  w->right->color = BLACK;
	}
	//Left rotate x's parent and we are done
	leftRotate(root, x->parent);
	x = root;
      }
    } else { //If x is a right child
      if (w->color == RED) { //Case 1: x is black and its sibling w is red
	//Color w black
	w->color = BLACK;
	//Color x's parent red
	x->parent->color = RED;
	//Right rotate x's parent
	rightRotate(root, x->parent);
	//Set w to x's parent's left
	w = x->parent->left;
	//Proceed to case 2, 3, or 4
      }

      if ((w->left == NULL || w->left->color == BLACK)
	  && (w->right == NULL || w->right->color == BLACK)) { //Case 2: x is black, its sibling w is black, and both of w's children are black
	//Color w red
	w->color = RED;
	//Set x to x's parent
	x = x->parent;
	//If new x is red, color x black (exit while loop) and we are done
	//If new x is black, go to case 1, 2, 3, or 4 (stay in while loop)
      } else if (w->left == NULL || w->left->color == BLACK) { //Case 3: x is black, sibling w is black, w's right child is red (implied), w's left child is black
	//Color w's right child black
	w->right->color = BLACK;
	//Color w red
	w->color = RED;
	//Let rotate w
	leftRotate(root, w);
	//Set w to x's parent's left
	w = x->parent->left;
	//Now stay in while loop to proceed to case 4
      } else  { //Case 4: x is black, sibling w is black, w's left child is red
	//Color w the same color as x's parent
	w->color = x->parent->color;
	//Color x's parent black
	x->parent->color = BLACK;
	//Color w's left child black
	if (w->left != NULL) {
	  w->left->color = BLACK;
	}
	//Right rotate x's parent and we are done
	rightRotate(root, x->parent);
	x = root;
      }
    }
  }
  x->color = BLACK; //Keeps root black

  /*else if (x->color == BLACK && w->color == RED) { //Case 1: x is black and its sibling w is red
    //Color w black
    w->color = BLACK;
    //Color x's parent red
    x->parent->color = RED;

    //Rotate x's parent
    if (x == x->parent->left) { //If x is the left child do a left rotation

    } else if (x == x->parent->right) { //If x is the right child do a right rotation

    }

  } else if (x->color == BLACK && w->color == BLACK
	     && w->left->color == BLACK && w->right->color == BLACK) { //Case 2: x is black and its sibling w is black and both of w's children are black

  } else if (x->color == BLACK && w->color == BLACK
	     && x == x->parent->left && w->left->color == RED && w->right->color == BLACK) { //Case 3a: x is black, its sibling w is black, x is the left child, w's left child is red, and w's right child is black

  } else if (x->color == BLACK && w->color == BLACK
	     && x == x->parent->right && w->right->color == RED && w->left->color == BLACK) { //Case 3b: x is black, its sibling w is black, x is the right child, w's right child is red, and w's left child is black

  } else if (x->color == BLACK && w->color == BLACK
	     && x == x->parent->left && w->right->color == RED) { //Case 4a: x is black, its sibling w is black, x is the left child, and w's right child is red

  } else if (x->color == BLACK && w->color == BLACK
	     && x == x->parent->right && w->left->color == RED) { //Case 4b: x is black, its sibling w is black, x is the right child, and w's left child is red

	     }*/
    
}

//Function removes the "NULL" nodes (with data = 0) that were created in the remove function 
Node* removeNULL(Node* & root) {
  Node* current = root;
  if (current == NULL) {
    return NULL;
  }
  current->right = removeNULL(current->right);
  if (current->data == 0) {
    delete current;
    return NULL;
  }
  current->left = removeNULL(current->left);
  return current;
}

//Function finds the minimum node in a tree or subtree
//Used in the REMOVE function to find the next largest integer
Node* findMin(Node* root) {
  if (root->left == NULL) {
    return root;
  } else {
    return findMin(root->left);
  }
}

//Function returns sibling of a node
Node* getSibling(Node* sibling) {
  if (sibling->parent == NULL) {
    return NULL;
  }
  if (sibling == sibling->parent->left) {
    return sibling->parent->right;
  } else if (sibling == sibling->parent->right) {
    return sibling->parent->left;
  }
}

//Created an extra function to clear the entire tree, for testing purposes :)
void CLEAR(Node* & root) {
  if (root == NULL) {
    return;
  }

  CLEAR(root->left);
  CLEAR(root->right);

  delete root;
  root = NULL;
}
