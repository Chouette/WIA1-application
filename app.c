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
#define VERBOSE 1

int nb_winners = 10 ;
char* output_file = NULL ;

/*
 * Compte le nombre de fichiers réguliers d'un dossier
 * @param path : le chemin vers le dossier
 * @param dir : la structure représentant le dossier
 * @return le nombre de fichiers réguliers présents dans le dossier
 */
int count_reg_files(const char* path, DIR* dir)
{
	struct stat st ;
	struct dirent* entite ;
	int count = 0 ;
	long pos ;
	char* copie = malloc((strlen(path) + 1) * sizeof(char)) ;
	char* racine = malloc((strlen(path) + 1) * sizeof(char)) ;

	if( copie == NULL || racine == NULL)
	{
		perror("malloc") ;
		exit(errno) ;
	}

	copie = strcpy( copie, path) ;
	racine = strcpy( racine, path) ;


	pos = telldir(dir) ;

	while( (entite = readdir(dir)) != NULL)
	{
		copie = strcat(copie, "/") ;
		copie = strcat(copie, entite->d_name) ;
		
		if( stat(copie, &st) == -1)
		{
			perror(path) ;
			exit(errno) ;
		}

		if( S_ISREG(st.st_mode) )
		{
			count++ ;
		}

		copie = strcpy( copie, racine) ;
	}

	
	seekdir(dir, pos) ;
	return count ;

}

/*
 * Traite les différentes options du programme
 * @param argc : le argc de main
 * @param argv : le argv de main
 * @return nombre d'arguments traités
 */
int parse_options(const int argc, char** argv)
{
	int i ;
	int c = 0 ;
	char* p ;

	for(i = 1 ; i < argc ; )
	{
		p = argv[i] ;

		if(p[0] == '-')
		{
			p++ ;

			switch(*p)
			{
				case 'h':
					printf("Syntaxe :\nappli -h\n"
						   "appli [-n nb_winners] <dossier_theme> [dossier_theme...]\n") ;
					exit(0) ;
					break ;
				case 'n':
					if(i+1 == argc)
					{
						fprintf(stderr, "missing argument for option -n\n") ;
						exit(-1) ;
					}
					p = argv[i+1] ;
					nb_winners = atoi(p) ;
					c += 2 ;
					i += 2 ;
					break ;
				case 'o' :
					if(i+1 == argc)
					{
						fprintf(stderr, "missing argument for option -n\n") ;
						exit(-1) ;
					}

					p = argv[i+1] ;
					output_file = calloc(strlen(p) + 1, sizeof(char) ) ;
					
					if(output_file == NULL)
					{
						perror("parse_options : calloc de output_file ") ;
					}
					else
					{
						output_file = strcpy(output_file, p) ;
					}

					c += 2 ;
					i += 2 ;
					break ;
				default:
					fprintf(stderr, "invalid argument -%c\n", *p) ;
					exit(-1) ;
					break ;
			}
		}
		else 
			return c ;
	}
	return c ;
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
	int count = 0, start ;
	FILE* fichierEnCours = NULL ;
	DIR* repTheme = NULL ;
	struct dirent* entite = NULL ;
	struct stat st ;
	HashTable* dico = NULL ;
	Mot *winners ;

	start = parse_options(argc, argv) ;
	winners = calloc(nb_winners, sizeof(Mot)) ;

	if(winners == NULL)
	{
		perror("main : calloc ") ;
		exit(errno) ;
	}
	

#if VERBOSE
	printf("Création du dictionnaire\n") ;
#endif
	dico = creer_dico(TAILLE) ;

	/* pour chaque dossier de thème */
	for(i = start + 1 ; i < argc ; i++)
	{
		printf("\n\n\n") ;
		printf("Thème %d sur %d : %s\n"
			   "-----------------------\n", i - start, argc -1, argv[i]) ;

		if(stat(argv[i], &st) == -1)
		{
			fprintf(stderr,"%s\n", argv[i]) ;
			perror("stat") ;
			continue ;
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

				/* Si on a pas encore rencontré le mot on l'ajoute au
				 * dictionnaire */
				if(!contient(dico, motLu))
				{
					if(i == start + 1)
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
						motAStocker.freq_app = 1.0 / count ;
						motAStocker.freq_thematique = 1.0 / (argc-1) ;
#if DEBUG
						printf("%s.freq_thematique = %f\n", motLu, motAStocker.freq_thematique) ;
#endif
						update_score(&motAStocker) ;
						motAStocker.dejaVu = j ;
						motAStocker.inTheme = i ;
#if VERBOSE
						printf("Ajout de \"%s\" au dictionnaire\n", motLu) ;
#endif
						insere(motAStocker, dico) ;
#if DEBUG
						printf("\n") ;
#endif
					}
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
					if(i == start + 1)
						motStocke.occurences++ ;

					/*
					 * si on a pas encore rencontré le mot dans le texte
					 */
					if(motStocke.dejaVu != j)
					{
#if DEBUG
						printf("if(motStocke.dejaVu != i)\n{\n") ;
#endif
						motStocke.dejaVu = j ;

						if(i == start + 1)
							motStocke.freq_app += 1.0 / count ;

						//printf("%s.freq_app = %f\n", motLu, motStocke.freq_app) ;
					}

					if( motStocke.inTheme != i)
					{
#if DEBUG
						printf("%s.inTheme = %d\n", motLu, motStocke.inTheme) ;
#endif
						motStocke.inTheme = i ;
						motStocke.freq_thematique += 1.0 / (argc-1) ;
#if DEBUG
						printf("%s.inTheme = %d, freq_thematique = %.2f\n", motLu, motStocke.inTheme, motStocke.freq_thematique) ;
#endif
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
		count = 0 ;
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

	for(i = 0 ; i < nb_winners ; i++)
		winners[i] = init_mot() ;

	printf("\tparcours du dictionnaire\n");
	for(i = 0 ; i < TAILLE ; i++)
	{
		ListeMots* liste ;

		liste = dico->contenu[i] ;
		
//		printf("\tanalyse de la page %d du dictionnaire\n", i) ;
		while (liste->suivant != NULL)
		{
			if(liste->element.score > winners[minIndex].score)
			{
				int j = 0 ;
				winners[minIndex] = liste->element ;
				
				/* recalculer l'index du minimum du tableau winners */
				for(j = 0 ; j < nb_winners ; j++)
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
	for(i = 0 ; i < nb_winners ; i++)
	{
		if(winners[i].mot[0] != '\0')
		{
			printf("\t- ") ; print_mot(winners[i]) ; printf("\n") ;
		}
	}

#if 1
	printf("\nObtenu à partir de la hash table suivante :\n") ;
	print_dico(dico) ;
#endif

	printf("\n********************\n"
		     "*generation du XML *\n"
		     "********************\n");
	if(generate_xml(winners, nb_winners, (output_file == NULL ? "words.xml" : output_file)) == 0)
		printf("fichier enregistré\n") ;
	else
		printf("erreur lors de la génération du fichier %s\n",output_file) ;

	return 0 ;
}

