#include "files.h"
#include "mot.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Liste des caractères considérés comme séparateurs de mots */
const static char separateurs[NB_SEP] = {
	'+'	,	'='		,	' '	,	'\t'	,	'\n', 
	','	,	';'		,	'.'	,	'?'		,	'!', 
	':'	, 	'\''	,	'('	,	')'		,	'/',
	'\\',	EOF		,	'&'	,	'"'		,	'\'',
	'#'	,	'{'		,	'['	,	'-'		,	'|',
	'_'	,	'@'		,	']'	,	'}'		,	'<',
	'>'} ;

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

	/* Ignorer tous les séparateurs qu'il y a entre 2 mots */
	while(is_separateur(c = getc( fichier))) 
	{
		if(c == EOF)
		{
			return NULL ;
		}
	}

	fseek (fichier, -1, SEEK_CUR) ;
	c = getc(fichier) ;

	/* Ajouter un à un les caractères du mot dans mot */
	while (!is_separateur(c))
	{
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

/*
 * Met un mot en minuscules
 * @param mot : le mot à mettre en minuscules
 * @return le mot en minuscule
 */
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

/*
 * Génère du code XML pour ThemeEditor
 * @param winners[] : tableau des mots à mettre dans le fichier XML
 * @param nbElements : taille de winners[]
 * @param output : le nom du fichier à générer
 * @return 1 si erreur, 0 sinon
 */
int generate_xml(Mot winners[], int nbElements, char* output)
{
	FILE* words = NULL ;
	int i = 0, ret = 1 ;
	
	words = fopen(output,"w") ;

	if(words == NULL)
	{
		fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", output) ;
		return 1 ;
	}

	for(i = 0 ; i <  nbElements ; i++)
	{
		if(winners[i].mot[0] != '\0')
		{
			if(fputs("<Word>", words) == EOF) 
				return 1 ;

			if(fputs(winners[i].mot, words)== EOF) 
				return 1 ;

			if(fputs("</Word>\n", words) == EOF) 
				return 1 ;
		}
	}

	return 0 ;
}


