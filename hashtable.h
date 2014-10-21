#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "liste.h"
#define TAILLE 5

typedef struct HashTable HashTable ;
struct HashTable
{
	unsigned int taille ;
	ListeMots** contenu ;
} ;

int hash(const char* cle) ;
HashTable* creer_dico(int tll) ;
int contient(HashTable* dico, Mot mot) ;
HashTable* insere(Mot mot, HashTable* dico) ;
void print_dico(HashTable* dico) ;
//HashTable redimensionner(Hashtable dico) ;

#endif
