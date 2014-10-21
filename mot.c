#include "mot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	mot.mot = (char*) malloc(TAILLE_MOT * sizeof(char)) ;
	mot.freq_app = mot.occurences = mot.score = 0 ;
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

Mot copie_mot(Mot* dest, const Mot* src) 
{
	dest->mot = strcpy(dest->mot, src->mot) ;
	dest->freq_app = src->freq_app ;
	dest->occurences = src->occurences ;
	dest->score = src->score ;
	return *dest ;
}
