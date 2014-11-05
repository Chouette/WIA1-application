#include "liste.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define VERBOSE 0
#define DEBUG 0

/*
 * Crée une liste de Mot par défaut
 * @return une liste de Mot vide
 */
ListeMots* init_listemots()
{
	ListeMots* liste ;

	liste = malloc(sizeof(struct ListeMots)) ;

	if(liste == NULL)
	{
		perror("init_listemots\n") ;
		exit(errno) ;
	}

#if DEBUG
	printf("init_listemots : initialisation de liste->element\n") ;
#endif
	liste->element = init_mot() ;
	liste->suivant = NULL ;

#if DEBUG
	if(liste == NULL)
		printf("init_listemots : something wrong happened\n") ;
#endif


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
	if(liste == NULL)
	{
		printf("liste inexistante\n") ;
		exit(-1) ;
	}

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
int in_list(char* mot, ListeMots* liste)
{
	ListeMots* tmp ;

#if DEBUG
	printf("in_list : initialisation de tmp\n") ;
#endif
	tmp = init_listemots() ;

	tmp = liste ;

#if DEBUG
	printf("in_list : début de la recherche\n") ;
#endif
	while(tmp->suivant != NULL)
	{
		if(strcmp(mot, (tmp->element).mot) == 0)
			return 1 ;

		tmp = tmp->suivant ;
	}

#if DEBUG
	printf("in_list : mot non trouvé\n") ;
#endif

	return 0 ;
}

/*
 * Recherche un mot dans une liste et le renvoie
 * @param mot : le mot recherché
 * @param liste : la liste dans laquelle on cherche
 * @return le mot vide si le mot cherché n'est pas dans la liste, le mot recherché sinon
 */
Mot list_get(char* mot, ListeMots* liste)
{
	Mot res = init_mot() ;

	while(liste->suivant != NULL)
	{
		if(strcmp(liste->element.mot, mot) == 0)
		{
			return liste->element ;
		}
		liste = liste->suivant ;
	}

	return res ;
}
