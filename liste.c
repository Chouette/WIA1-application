#include "liste.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * Crée une liste de Mot par défaut
 * @return une liste de Mot vide
 */
ListeMots* init_listemots()
{
	ListeMots* liste ;
	liste = malloc(sizeof(struct ListeMots)) ;
	liste->element = init_mot() ;
	liste->suivant = NULL ;

	return liste ;
}

/*
 * Ajoute un Mot dans une ListeMots
 * @param elmnt : le Mot à ajouter
 * @param liste : la liste dans laquelle on ajoute l'élément
 * @return la liste avec en tête le nouvel élément
 */
ListeMots* add(const Mot elmnt, ListeMots* liste)
{
	ListeMots* tmp = init_listemots() ;
	tmp->element =  copie_mot(&(tmp->element), &elmnt) ;
	tmp->suivant = liste ;

	return tmp ;
}

/*
 * Affiche le contenu d'une liste de mots
 * @param liste : la liste à afficher
 */
void print_listemot(ListeMots* liste)
{
	while(liste->suivant != NULL)
	{
		print_mot(liste->element) ;
		printf(" -->  ") ;
		liste = liste->suivant ;
	}

	printf("NULL\n") ;
}

/*
 * Détermine si un mot fait partie de la liste
 * @param mot : le mot à rechercher
 * @param liste : la liste dans laquelle chercher
 * @return 1 si mot est dans liste, 0 sinon
 */
int in_list(Mot mot, ListeMots* liste)
{
	ListeMots* tmp = init_listemots() ;
	tmp = liste ;

	while(tmp->suivant != NULL)
	{
		if(strcmp(mot.mot, (tmp->element).mot) == 0)
			return 1 ;
		tmp = tmp->suivant ;
	}

	return 0 ;
}
