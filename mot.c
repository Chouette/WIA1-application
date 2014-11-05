#include "mot.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERBOSE 0

/*
 * Affiche les composantes du mot
 * @param mot : le mot qu'on veut afficher
 */
void print_mot(Mot mot)
{
	printf("{\"%s\", %.2f, %d, %.2f}", mot.mot, mot.freq_app, mot.occurences, mot.score) ;
}

/*
 * crée un Mot vide
 * @return un Mot vide
 */
Mot init_mot()
{
	Mot mot ;

#if VERBOSE
	printf("init_mot : création d'un mot vide\n") ;
#endif
	mot.mot = malloc(TAILLE_MOT * sizeof(char)) ;

	if(mot.mot == NULL)
	{
		perror("init_mot") ;
		exit(errno) ;
	}

	mot.freq_app = mot.occurences = mot.score = mot.dejaVu = 0  ;
#if VERBOSE
	printf("init_mot : mot créé :") ;
	print_mot(mot) ;
	printf("\n") ;
#endif
	return mot ;
}

/*
 * Met à jour le score du mot
 * @param mot : le mot dont on souhaite mettre le score à jour
 */
void update_score (Mot* mot)
{
	mot->score = mot->freq_app * mot->occurences ;
}

/*
 * Copie un mot dans un autre
 * @param dest : la copie
 * @param src : l'original à copier
 * @return la copie
 */
Mot copie_mot(Mot* dest, const Mot* src) 
{
	dest->mot = strcpy(dest->mot, src->mot) ;
	dest->freq_app = src->freq_app ;
	dest->occurences = src->occurences ;
	dest->score = src->score ;
	dest->dejaVu = src->dejaVu ;
	return *dest ;
}
