#ifndef MOT_H
#define MOT_H

#define TAILLE_MOT 50

typedef struct Mot Mot ;
struct Mot
{
	char* mot ;
	double freq_app ;
	double freq_thematique ;
	int occurences ;
	int dejaVu ;
	int inTheme ;
	double score ; //score = freq_app * occurences
} ;

void print_mot(Mot mot) ;
Mot init_mot() ;
void update_score(Mot* mot) ;
Mot copie_mot(Mot* dest, const Mot* src) ;

#endif
