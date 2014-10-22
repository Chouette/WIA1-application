#include "files.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define DEBUG 0

const static char separateurs[NB_SEP] = {' ', '\t', '\n', ',', ';', '.', '?', '!', ':', '\'', '(', ')', EOF} ;
/*
 * Indique si un caractère est un séparateur de mots
 * @param c : le caractère à tester
 * @return 1 si c est un séparateur, 0 sinon
 */
int is_separateur (char c)
{
	int i = 0 ;
	
	for(i = 0 ; i < NB_SEP ; i++)
	{
		if (c == separateurs[i])
			return 1 ;
	}

	return 0 ;
}

/*
 * Lit le mot suivant dans fichier
 * @param fichier : le fichier qu'on dans lequel on lit
 * @return le mot lu, ou NULL si on atteint la fin du fichier
 */
char* lire_mot (FILE* fichier, char* mot)
{
	char c = 'a' ;
	int i = 0 ;

	while(is_separateur(c = getc( fichier))) 
	{
#if DEBUG
		printf("while(is_separateur(c = getc( fichier)))\n") ; 
#endif
		if(c == EOF)
		{
#if DEBUG
			printf("\tif(c == EOF)\n") ;
#endif
			return NULL ;
		}
	}
	fseek (fichier, -1, SEEK_CUR) ;

	c = getc(fichier) ;
	while (!is_separateur(c))
	{
#if DEBUG
		printf("while (!is_separateur(c))\n{\n") ;
#endif
		mot[i++] = c ;
		c = getc(fichier) ;
		if(c == EOF)
		{
			return NULL ;
		}
	}
	mot[i] = '\0' ;
	return mot ;
}

char* to_lower(char* mot)
{
	int i = 0 ;

	if(!mot)
	{
		return mot ;
	}
	else
	{
		char* c = malloc(strlen(mot) + 1);
		while(*mot != '\0')
		{
			c[i++] = tolower(*mot) ;
			mot++ ;
		}
		c[i] = '\0' ;
		return c ;
	}
}

