#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LinkList.h"

typedef struct{
  char ma[25];
  node *position;
  int timesOfApperance; 
} wordindex;
  
typedef wordindex KeyType;
typedef struct Node{
  KeyType key; 
  struct Node *left, *right;
}NodeType;

typedef NodeType* TreeType;

TreeType Search(char *x,TreeType Root);
void insertNode(KeyType d,TreeType *Root);
void inorderprint(TreeType tree);
void DeleteNode(KeyType X,TreeType *Root);
void freetree(TreeType tree);

TreeType Search(char *x,TreeType Root){ 
  if (Root == NULL)
    return NULL; 
  else if (strcmp(Root->key.ma, x) == 0)
    return Root; 
  else if (strcmp(Root->key.ma, x) < 0)	
    return Search(x, Root->right); 
  else{
    return Search(x, Root->left); 
  }
}

void insertNode(KeyType d,TreeType *Root){ 
  if (*Root == NULL){
    *Root = (NodeType*)malloc(sizeof(NodeType)); 
    (*Root)->key = d;
    (*Root)->left = NULL; 
    (*Root)->right = NULL;
  } 
  else if (strcmp(d.ma,(*Root)->key.ma) < 0)
    insertNode(d, &(*Root)->left);
  else if (strcmp(d.ma,(*Root)->key.ma) > 0)
    insertNode(d, &(*Root)->right);
}

void inorderprint(TreeType tree){
  if(tree != NULL){
    inorderprint(tree->left);
    printf("%-25s: ", tree->key.ma);
    printf("%d,",tree->key.timesOfApperance);
    getLinkData(tree->key.position);
    printf("\n");
    inorderprint(tree->right);
  }
}


KeyType DeleteMin (TreeType *Root){
  KeyType k;
  if ((*Root)->left == NULL){
    k=(*Root)->key;
    (*Root) = (*Root)->right;
    return k;
  }
  else return DeleteMin(&(*Root)->left);
}

void DeleteNode(KeyType x, TreeType *Root) {
  if (*Root != NULL){
    if (strcmp(x.ma,(*Root)->key.ma) < 0) {
      DeleteNode(x, &(*Root)->left);
    }
    else if (strcmp(x.ma,(*Root)->key.ma) > 0) {
      DeleteNode(x, &(*Root)->right);
    }
    else if ((*Root)->left == NULL && (*Root)->right == NULL) {
      (*Root) = NULL;
    }
    else if ((*Root)->left == NULL){
      *Root = (*Root)->right;
    }
    else if ((*Root)->right==NULL){
      *Root = (*Root)->left;
    }
    else (*Root)->key = DeleteMin(&(*Root)->right);
  }
}

void freetree(TreeType tree)
{
if (tree!=NULL)
  {
    freetree(tree->left);
    freetree(tree->right);
    free((void *) tree);
  }
}