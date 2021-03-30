#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef int elementtype; 
typedef struct node node;
typedef struct node{ 
  	elementtype element; 
  	node *next; 
} node; 

node *cur;


node *makenewNode(elementtype addr);
void insertTail(elementtype addr, node *root);
void getLinkData(node *root);
void freelist(node *root);

node *makenewNode(elementtype addr){
	node *newNode = (node*)malloc(sizeof(node));
	newNode->element = addr;
	newNode->next = NULL;
	return newNode;
}

void insertTail(elementtype addr, node *root){
	node *newNode = makenewNode(addr);
  	if(root == NULL)
    	root = newNode;
  	else{
    	cur = root;
    	while(cur->next != NULL){
      		cur = cur->next;
    	}
    	cur->next = newNode;
  	}
}

void getLinkData(node *root){
  	for (node *p = root; p != NULL; p = p->next){
    	printf("%d,", p->element);
  	}
}

void freelist(node *root){
	node *del = root;
  	while(root != NULL){
    	root = root->next;
    	free(del);
    	del = root;
  	}
}