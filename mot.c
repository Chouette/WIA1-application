#include "mot.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define VERBOSE 0

/*
 * Affiche les composantes du mot
 * @param mot : le mot qu'on veut afficher
 */
void print_mot(Mot mot)
{
	printf("{\"%s\", f_a = %.2f, f_t = %.2f, %d occ, score : %.2f}", mot.mot, mot.freq_app, mot.freq_thematique, mot.occurences, mot.score) ;
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
	mot.mot[0] = '\0' ;

	mot.freq_app = mot.occurences = mot.dejaVu = mot.freq_thematique = mot.inTheme = 0  ;
	mot.score = 0 ;
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
	mot->score = -log(mot->freq_thematique) * mot->freq_app * mot->occurences ;
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
	dest->freq_thematique = src->freq_thematique ;
	dest->occurences = src->occurences ;
	dest->score = src->score ;
	dest->dejaVu = src->dejaVu ;
	dest->inTheme = src->inTheme ;

	return *dest ;
}

