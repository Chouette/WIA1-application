#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "files.h"
#include "mot.h"
#include "liste.h"
#include "hashtable.h"
#define NEWLINE() printf("\n") ;


int nb_winners = 10 ;

void parse_arguments(const int argc, char** argv)
{
	int i ;
	char* p ;

	for(i = 1 ; i < argc ; i++)
	{
		p = argv[i] ;
		if(p[0] == '-')
		{
			p++ ;

			switch(*p)
			{
				case 'h':
					printf("Syntaxe : \nappli -h\n"
						   "appli [-n nb_winners] <dossier_theme> [dossier_theme...]\n") ;
					exit(0) ;
					break ;
				case 'n':
					if(i+1 == argc)
					{
						fprintf(stderr, "missing argument for option -n\n") ;
						exit(-1) ;
					}
					p = argv[i+1] ;
					nb_winners = atoi(p) ;
					break ;
				default:
					fprintf(stderr, "invalid argument -%c\n", *p) ;
					exit(-1) ;
					break ;
			}
			printf("%s\n", p) ;
		}
	}
}

int main(int argc, char** argv)
{
	FILE* fileTest = NULL ;
	char* mot ;
	Mot word ;
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
	if(contient(dico,"trololo"))
			printf("trololol\n") ;
	printf("\n\n") ;

	printf("mot = %s, hash(mot) = %d\n", word.mot, hash(word.mot)) ;
	fclose(fileTest) ;
	printf("\n\n\n") ;


	printf("******************\n"
		   "*TEST ignore case*\n"
		   "******************\n") ;
	{
		char* s = "couCOU" ;

		printf("%s -> %s\n", s, to_lower(s)) ;
	}

	printf("\n\n\n") ;
	printf("*******************\n"
		   "*TEST generate_xml*\n"
		   "*******************\n") ;
	{
		Mot winners[10] ;
		int i = 0 ;

		for(i = 0 ; i < 10 ; i++)
		{
			winners[i] = init_mot() ;
			winners[i].mot = "coucou" ;
		}

		generate_xml(winners, 10) ;
	}

	printf("\n\n\n") ;
	printf("*******************\n"
		   "*TEST parse_arguments*\n"
		   "*******************\n") ;
	{
		parse_arguments(argc, argv) ;
		printf("nb_winners = %d\n", nb_winners) ;
	}
	return 0 ;
}

