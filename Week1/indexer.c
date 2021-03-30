#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "BinaryTree.h"


#define STOP 1000
#define MAX 100000
void toLower(char *str)
{
	for(int i = 0; i < strlen(str); i++) {
		str[i] = tolower(str[i]);
	}
}
int checkStop(char stop[STOP][81], char *word){
	int checkStop = 0;
	for(int i = 0; i < STOP; i++){
		if (strcmp(stop[i], word) == 0 ){
			checkStop = 1;
			break;
		}
	}
	return checkStop;
}

int main() {
	FILE *fptr1, *fptr2;
	
	fptr1= fopen("alice30.txt", "r");
	fptr2 = fopen("stopw.txt", "r");

	if (fptr1 == NULL || fptr2 == NULL) {
		printf("Cannot open file\n");
		return(0);
	}

	
	TreeType root = NULL;
	TreeType tmp;
	char stop[STOP][81];

	for (int i = 0; i < STOP; i++) {
		fscanf(fptr2,"%s\n", stop[i]);
	}



	// Read text
	char a;
	char wordList[MAX][81];
	char temp[81];
	int numtemp = 0, numWord = 0, numLine = 1;
	
	do {
		a = fgetc(fptr1);
		if (a == EOF)
			break;

		if (a == '\n'){
			numLine++;
		}


		if( (isalpha(a))  ){
			temp[numtemp] = a;
			numtemp++;
		}
		else {
			temp[numtemp] = '\0';
				
			toLower(temp);
			if (checkStop(stop,temp) == 1)
			{
				strcpy(temp,"");
				numtemp = 0;
			}
			else {
			strcpy(wordList[numWord],temp);
			if (Search(wordList[numWord], root) == NULL){
				KeyType new;
				strcpy(new.ma,wordList[numWord]);
				new.position = makenewNode(numLine);
				insertNode(new, &root);
				new.timesOfApperance = 1;

			}
			else {
				tmp = Search(wordList[numWord], root);
				insertTail(numLine,tmp->key.position);
				tmp->key.timesOfApperance +=1 ;
			}
			numWord++;
			strcpy(temp,"");
			numtemp = 0;
			}
		}
			
	}  while(1);


	inorderprint(root);
	fclose(fptr1);
	fclose(fptr2);
	freetree(root);
	return 0;
} 