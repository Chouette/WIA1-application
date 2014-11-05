#include "mot.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

	mot.mot = malloc(TAILLE_MOT * sizeof(char)) ;

	if(mot.mot == NULL)
	{
		perror("init_mot") ;
		exit(errno) ;
	}

	mot.mot[0] = '\0' ;
	mot.freq_app = mot.occurences = mot.dejaVu = mot.freq_thematique = mot.inTheme = 0  ;
	mot.score = 0 ;

	return mot ;
}

/*
 * Met à jour le score du mot
 * @param mot : le mot dont on souhaite mettre le score à jour
 * @param singleMode : indique de prendre en compte ou non la fréquence
 * thématique dans le calcul du score
 */
void update_score (Mot* mot, int singleMode)
{
	mot->score = (!singleMode ? -log(mot->freq_thematique) * mot->freq_app * mot->occurences : mot->freq_app * mot->occurences) ;
}

/*
 * Copie un mot dans un autre, champ par champ
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

