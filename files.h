#ifndef FILES_H
#define FILES_H

#define NB_SEP 31

#include <stdio.h>

int is_separateur (char c) ;
char* lire_mot (FILE* fichier, char* mot) ;
char* to_lower(char* mot) ;

#endif

