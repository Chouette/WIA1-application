#ifndef LISTE_H
#define LISTE_H

#include "mot.h"

typedef struct ListeMots ListeMots ;
struct ListeMots
{
	Mot element ;
	ListeMots* suivant ;
} ;

ListeMots* add(const Mot elmnt, ListeMots* liste) ;
ListeMots* init_listemots() ;
void print_listemot(ListeMots* liste) ;
int in_list(Mot mot, ListeMots* liste) ;
#endif
