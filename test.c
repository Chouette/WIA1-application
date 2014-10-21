#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "files.h"
#include "mot.h"
#include "liste.h"
#include "hashtable.h"
#define NEWLINE() printf("\n") ;

int main(int argc, char** argv)
{
	FILE* fileTest = NULL ;
	char* mot ;
	Mot word, trololo ;
	ListeMots* liste ;
	HashTable* dico ;

	if(argc < 1)
		return 1 ;

	printf("*****************\n"
		   "*TEST lire_mot()*\n"
		   "*****************\n") ;

	fileTest = fopen(argv[1] ,"r") ;
	if(fileTest == NULL)
	{
		printf("no good : the file couldn't be opened !\n") ;
		exit(1) ;
	}

	mot = malloc(50 * sizeof(char)) ;

	mot = lire_mot(fileTest, mot) ;
	while (mot != NULL)
	{
		printf("%s ", mot) ;
		mot = lire_mot(fileTest, mot) ;
	}
	free(mot) ;
	printf("\n\n\n") ;


	printf("********************\n"
		   "*TEST structure Mot*\n"
		   "********************\n") ;
	rewind(fileTest) ;
	word = init_mot() ;
	word.mot = lire_mot(fileTest, word.mot) ;
	print_mot(word) ;
	printf("\n") ;
	word.freq_app+=3 ;
	word.occurences+=2 ;
	update_score(&word) ;
	print_mot(word) ;
	printf("\n\n\n") ;


	printf("**************************\n"
		   "*TEST structure ListeMots*\n"
		   "**************************\n") ;
	liste = init_listemots() ;
	print_listemot(liste) ;
	printf("\n\n") ;

	word.mot = strcpy(word.mot, lire_mot(fileTest, word.mot)) ;
	word.freq_app = 3 ;
	word.occurences = 2 ;
	update_score(&word) ;
	liste = add(word, liste) ;
	print_listemot(liste) ;
	printf("\n\n") ;

	word.mot = strcpy(word.mot, lire_mot(fileTest, word.mot)) ;
	word.freq_app = 6 ;
	word.occurences = 4 ;
	update_score(&word) ;
	liste = add(word, liste) ;
	print_listemot(liste) ;
	printf("\n\n") ;

	word.mot = strcpy(word.mot, lire_mot(fileTest, word.mot)) ;
	word.freq_app = 12 ;
	word.occurences = 8 ;
	update_score(&word) ;
	liste = add(word, liste) ;
	print_listemot(liste) ;
	printf("\n\n\n") ;


	printf("**************************\n"
		   "*TEST structure HashTable*\n"
		   "**************************\n") ;
	dico = creer_dico(TAILLE) ;
	word.mot = strcpy(word.mot, lire_mot(fileTest, word.mot)) ;
	word.freq_app = 1 ;
	word.occurences = 2 ;
	update_score(&word) ;
	dico = insere(word, dico) ;
	print_dico(dico) ;
	printf("\n\n") ;

	word.mot = strcpy(word.mot, lire_mot(fileTest, word.mot)) ;
	word.freq_app = 3 ;
	word.occurences = 5 ;
	update_score(&word) ;
	dico = insere(word, dico) ;
	print_dico(dico) ;
	printf("\n\n") ;

	word.mot = strcpy(word.mot, lire_mot(fileTest, word.mot)) ;
	word.freq_app = 8 ;
	word.occurences = 13 ;
	update_score(&word) ;
	dico = insere(word, dico) ;
	print_dico(dico) ;
	trololo = init_mot() ;
	trololo.mot = strcpy(trololo.mot, "trololo") ;
	if(contient(dico,trololo))
			printf("trololol\n") ;
	printf("\n\n") ;

	printf("mot = %s, hash(mot) = %d\n", word.mot, hash(word.mot)) ;
	fclose(fileTest) ;
	return 0 ;
}

