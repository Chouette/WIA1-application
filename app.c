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
#define DEBUG 0
#define VERBOSE 0

/*
 * Compte le nombre de fichiers réguliers d'un dossier
 * @param path : le chemin vers le dossier
 * @param dir : la structure représentant le dossier
 * @return le nombre de fichiers réguliers présents dans le dossier
 */
int count_reg_files(char* path, DIR* dir)
{
	struct stat st ;
	struct dirent* entite ;
	int count = 0 ;
	long pos ;
	char* copie = malloc((strlen(path) + 1) * sizeof(char)) ;

	if( copie == NULL)
	{
		perror("malloc") ;
		exit(errno) ;
	}

	copie = strcpy( copie, path) ;

	pos = telldir(dir) ;

	while( (entite = readdir(dir)) != NULL)
	{
		path = strcat(path, "/") ;
		path = strcat(path, entite->d_name) ;
		
		if( stat(path, &st) == -1)
		{
			perror(path) ;
			exit(errno) ;
		}

		if( S_ISREG(st.st_mode) )
			count++ ;
		path = strcpy( path, copie) ;
	}

	
	seekdir(dir, pos) ;
	return count ;

}

/*
 * Le programme génère une liste de mots supposés être représentatifs d'un
 * thème donné. Pour cela il prend en argument une liste de dossiers (au moins
 * 2),
 * représentant chacun un thème. Le premier dossier est celui du thème que l'on
 * considère. Les autres dossier servent à effectuer des comparaisons entre les
 * thèmes
 */
int main(int argc, char** argv)
{
	int i = 0, minIndex = 0, nouveauxMots = 0, maj = 0, j = 1 ;
	int count ;
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
		{
#if VERBOSE
			printf("paramètre ignore : %s n'est pas un dossier\n", argv[i]) ;
#endif
			continue ;
		}


#if VERBOSE
		printf("Ouverture du dossier %s\n", argv[i]) ;
#endif
		repTheme = opendir(argv[i]) ;

		if(repTheme == NULL)
		{
			perror("") ;
			exit(errno) ;
		}

		count = count_reg_files(argv[i], repTheme) ;

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

			if(stat(path, &st) == -1)
			{
				perror(path) ;
				continue ;
			}

			if(!S_ISREG(st.st_mode))
			{
#if VERBOSE
				printf("element ignore : %s n'est pas un fichier regulier\n", path) ;
#endif
				continue ;
			}

#if VERBOSE
			printf("Ouverture du fichier %s\n", entite->d_name) ;
#endif

			fichierEnCours = fopen(path, "r") ;

			if(fichierEnCours == NULL)
			{
				fprintf(stderr, "Impossible d'ouvrir %s\n", entite->d_name) ;
				perror("") ;
				exit(errno) ;
			}

			printf("***********************\n"
				   "*Lecture du fichier %s*\n"
				   "***********************\n", entite->d_name) ;
			++j ;

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
					Mot motAStocker = init_mot() ;
					nouveauxMots++ ;
#if DEBUG
					printf("if(!contient(dico, motLu))\n{\n");
#endif

#if VERBOSE
					printf("le dictionnaire ne contient pas \"%s\"\n",motLu) ;
#endif
#if DEBUG
					printf("motAStocker.mot = strcpy(motAStocker.mot, motLu)\n");
#endif
					motAStocker.mot = strcpy(motAStocker.mot, motLu) ;
					motAStocker.occurences = 1 ;
					motAStocker.freq_app = 1.0/count ;
					update_score(&motAStocker) ;
					motAStocker.dejaVu = j ;
#if VERBOSE
					printf("Ajout de \"%s\" au dictionnaire\n", motLu) ;
#endif
					insere(motAStocker, dico) ;
#if DEBUG
					printf("\n") ;
#endif
				}
				else
				{
					Mot motStocke ;

					maj++ ;
#if DEBUG
					printf("else\n{//contient(dico,motLu)\n") ;
#endif
					motStocke = get(motLu, dico) ;
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
						motStocke.freq_app += 1.0 / count ;
					}

					update_score(&motStocke) ;
					set(motStocke, dico) ;
#if DEBUG
					printf("mot stocke mis à jour\n") ;
					printf("\n") ;
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
		
//		printf("\tanalyse de la page %d du dictionnaire\n", i) ;
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

#if DEBUG
	printf("\nObtenu à partir de la hash table suivante :\n") ;
	print_dico(dico) ;
#endif

	printf("\n********************\n"
		     "*generation du XML *\n"
		     "********************\n");
	generate_xml(winners, NB_WINNERS) ;
	printf("fichier enregistré\n") ;

	return 0 ;
}

