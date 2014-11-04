#include <stdlib.h>
#include <string.h>
#include "files.h"
#include "mot.h"
#include "hashtable.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>

#define NB_WINNERS 10
#define DEBUG 1

int main(int argc, char** argv)
{
	int i = 0, minIndex = 0, nouveauxMots = 0, maj = 0, j = 1 ;
	FILE* fichierEnCours = NULL ;
	DIR* repTheme = NULL ;
	struct dirent* entite = NULL ;
	struct stat st ;
	HashTable* dico = NULL ;
	Mot winners[NB_WINNERS] ;
	

	dico = creer_dico(TAILLE) ;

	/* pour chaque dossier de thème */
	for(i = 1 ; i < argc ; i++)
	{
		if(stat(argv[i], &st) == -1)
		{
			perror("stat") ;
			exit(errno) ;
		}

		if(!S_ISDIR(st.st_mode))
			continue ;

		repTheme = opendir(argv[i]) ;

		if(repTheme == NULL)
		{
			perror("") ;
			exit(errno) ;
		}

		for(entite = readdir(repTheme) ; entite != NULL ; entite = readdir(repTheme))
		{
			char *path = NULL, *motLu = NULL ;

			motLu = malloc(50 * sizeof(char)) ;

			if(motLu == NULL)
			{
				perror("malloc") ;
				exit(errno) ;
			}

			path = malloc( (strlen(argv[i]) + strlen(entite->d_name) + 2) * sizeof(char)) ;

			if(path == NULL)
			{
				perror("malloc2") ;
				exit(errno) ;
			}

			path = strcat(path, argv[i]) ;
			path = strcat(path, "/") ;
			path = strcat(path, entite->d_name) ;
			fichierEnCours = fopen(path, "r") ;

			if(fichierEnCours == NULL)
			{
				fprintf(stderr, "Impossible d'ouvrir %s\n", entite->d_name) ;
				perror("") ;
				exit(errno) ;
			}

			printf("***********************\n"
				   "*Lecture du fichier %d*\n"
				   "***********************\n",j++) ;

			motLu = to_lower(lire_mot(fichierEnCours, motLu)) ;	

			/* Lire tout le texte */
			while(motLu != NULL)
			{
#if DEBUG
				printf("while(motLu != NULL)\n{\n") ;
#endif

				/* Si on a pas encore rencontré le mot */
				if(!contient(dico, motLu))
				{
					nouveauxMots++ ;
#if DEBUG
					printf("if(!contient(dico, motLu))\n{\n");
#endif
					Mot motAStocker = init_mot() ;
#if DEBUG
					printf("motAStocker.mot = strcpy(motAStocker.mot, motLu) ;\n");
#endif
					motAStocker.mot = strcpy(motAStocker.mot, motLu) ;
					motAStocker.occurences = 1 ;
					motAStocker.freq_app = 1.0/(argc-1) ;
					update_score(&motAStocker) ;
					motAStocker.dejaVu = j ;
					insere(motAStocker, dico) ;
				}
				else
				{
					maj++ ;
#if DEBUG
					printf("else\n{//contient(dico,motLu)\n") ;
#endif
					Mot motStocke = get(motLu, dico) ;
#if DEBUG
					printf("get terminé\n") ;
#endif
					motStocke.occurences++ ;

					if(motStocke.dejaVu != j)
					{
#if DEBUG
						printf("if(motStocke.dejaVu != i)\n{\n") ;
#endif
						motStocke.dejaVu = j ;
						motStocke.freq_app += 1.0 / (argc-1) ;
					}

					update_score(&motStocke) ;
					set(motStocke, dico) ;
#if DEBUG
					printf("mot stocke mis à jour\n") ;
#endif
				}

				motLu = to_lower(lire_mot(fichierEnCours, motLu)) ;	
			}//fin while

			printf("\t%d nouveaux mots enregistrés\n\t%d mises a jour\n",nouveauxMots, maj) ;
			nouveauxMots = 0 ;
			maj = 0 ;
#if DEBUG
			printf("fin while(motLu != NULL)\n") ;
#endif
		}
	}//fin for()

	printf("\n***********************\n"
		   "*Selection des winners*\n"
		   "***********************\n") ;

	/*
	 * En principe, à ce niveau on a enregistré tous les mots de tous les textes dans le dictionnaire.
	 * Il ne reste plus qu'à récupérer ceux ayant le plus gros score
	 * Algorithme :
	 * - créer un tableau de 10 mots
	 * - pour chaque mot du dictionnaire
	 * 		si son score est supérieur à celui du minimum du tableau, alors
	 * 			remplacer ce minimum
	 * 			recalculer le minimum du tableau
	 * 		fin si
	 *	 fin pour
	 */

	printf("\tinitialisation du tableau\n") ;
	for(i = 0 ; i < NB_WINNERS ; i++)
		winners[i] = init_mot() ;

	for(i = 0 ; i < TAILLE ; i++)
	{
		ListeMots* liste = dico->contenu[i] ;
		
		printf("\tanalyse de la page %d du dictionnaire\n", i) ;
		while (liste->suivant != NULL)
		{
			if(liste->element.score > winners[minIndex].score)
			{
				int j = 0 ;
				winners[minIndex] = liste->element ;
				
				/* recalculer l'index du minimum du tableau winners */
				for(j = 0 ; j < NB_WINNERS ; j++)
				{
					if(winners[j].score < winners[minIndex].score)
						minIndex = j ;
				}
			}
			liste = liste->suivant ;
		}
	}

	printf("\n*************\n"
		     "*Résultat : *\n"
		     "*************\n");
	for(i = 0 ; i < NB_WINNERS ; i++)
	{
		printf("\t- ") ; print_mot(winners[i]) ; printf("\n") ;
	}

	printf("\nObtenu à partir de la hash table suivante :\n") ;
	print_dico(dico) ;

	printf("\n********************\n"
		     "*generation du XML *\n"
		     "********************\n");
	generate_xml(winners, NB_WINNERS) ;
	printf("fichier enregistré\n") ;

	return 0 ;
}

