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

	/* Pour chaque fichier du dossier */
	while( (entite = readdir(dir)) != NULL)
	{
		/* copie := chemin complet du fichier */
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
	int i = 1 ;
	int c = 0 ;
	char* p ;

	while (i < argc)
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
	}// fin while

	return c ;
}

/*
 * Le programme génère une liste de mots supposés être représentatifs d'un
 * thème donné. Pour cela il prend en argument une liste de dossiers
 * représentant chacun un thème. Le premier dossier est celui du thème que l'on
 * considère. Les autres dossier servent à effectuer des comparaisons entre les
 * thèmes.
 * Les options -n et -o doivent être placées au début
 */
int main(int argc, char** argv)
{
	int i = 0, minIndex = 0, nouveauxMots = 0, maj = 0, j = 1 ;
	int count = 0, start, singleMode = 0 ;
	FILE* fichierEnCours = NULL ;
	DIR* repTheme = NULL ;
	struct dirent* entite = NULL ;
	struct stat st ;
	HashTable* dico = NULL ;
	Mot *winners ;

	if(argc <= 1)
	{
		printf("Syntaxe :\nappli -h\n"
		       "appli [-n nb_winners] [-o output] <dossier_theme> [dossier_theme...]\n") ;
		return 1 ;
	}

	start = parse_options(argc, argv) ;

	if(argc - start == 2)
	{
		singleMode = 1 ;
	}

	dico = creer_dico(TAILLE) ;

	/* pour chaque dossier de thème */
	for(i = start + 1 ; i < argc ; i++)
	{
		printf("\n\n\n") ;
		printf("Thème %d sur %d : %s\n"
			   "-----------------------\n", i - start, argc - start - 1, argv[i]) ;

		/* Vérifier que l'argument est bien un dossier */
		if(stat(argv[i], &st) == -1)
		{
			fprintf(stderr,"%s\n", argv[i]) ;
			perror("stat") ;
			continue ;
		}

		if(!S_ISDIR(st.st_mode))
		{
			continue ;
		}

		/* Commencer à travailler dans le dossier */
		repTheme = opendir(argv[i]) ;

		if(repTheme == NULL)
		{
			perror("") ;
			exit(errno) ;
		}

		count = count_reg_files(argv[i], repTheme) ;

		/* Pour chaque fichier du dossier */
		for(entite = readdir(repTheme) ; entite != NULL ; entite = readdir(repTheme))
		{
			char *path = NULL, *motLu = NULL ;

			motLu = malloc(50 * sizeof(char)) ;

			if(motLu == NULL)
			{
				perror("malloc") ;
				exit(errno) ;
			}

			/* Obtenir le chemin complet du fichier */
			path = malloc( (strlen(argv[i]) + strlen(entite->d_name) + 2) * sizeof(char)) ;

			if(path == NULL)
			{
				perror("malloc2") ;
				exit(errno) ;
			}

			path = strcat(path, argv[i]) ;
			path = strcat(path, "/") ;
			path = strcat(path, entite->d_name) ;

			/* Vérifier qu'il s'agit d'un fichier régulier */
			if(stat(path, &st) == -1)
			{
				perror(path) ;
				continue ;
			}

			if(!S_ISREG(st.st_mode))
			{
				continue ;
			}

			/* Commencer à travailler sur le fichier */
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

				/* Si on a pas encore rencontré le mot on l'ajoute au
				 * dictionnaire */
				if(!contient(dico, motLu))
				{
					/* Ne pas prendre en compte les nouveaux mots des thèmes
					 * autres que le thème considéré (celui du premier dossier,
					 * donc) */
					if(i == start + 1)
					{
						Mot motAStocker = init_mot() ;

						nouveauxMots++ ;
						motAStocker.mot = strcpy(motAStocker.mot, motLu) ;
						motAStocker.occurences = 1 ;
						motAStocker.freq_app = 1.0 / count ;
						motAStocker.freq_thematique = 1.0 / (argc - start - 1) ;
						update_score(&motAStocker, singleMode) ;
						motAStocker.dejaVu = j ;
						motAStocker.inTheme = i ;
						insere(motAStocker, dico) ;
					}
				}
				else
				{
					/*
					 * Si le mot est déjà dans le dictionnaire, le mettre à jour
					 * comme il se doit
					 */

					Mot motStocke ;

					maj++ ;
					motStocke = get(motLu, dico) ;

					if(i == start + 1)
						motStocke.occurences++ ;

					/*
					 * si on a pas encore rencontré le mot dans le texte, mettre
					 * à jour sa freq_app
					 */
					if(motStocke.dejaVu != j)
					{
						motStocke.dejaVu = j ;

						if(i == start + 1)
							motStocke.freq_app += 1.0 / count ;
					}

					/* 
					 * si le mot n'a pas été enregistré pour le thème, mettre à
					 * jour sa freq_thematique 
					 */
					if( motStocke.inTheme != i)
					{
						motStocke.inTheme = i ;
						motStocke.freq_thematique += 1.0 / (argc - start -1) ;
					}

					update_score(&motStocke, singleMode) ;
					set(motStocke, dico) ;
				}

				motLu = to_lower(lire_mot(fichierEnCours, motLu)) ;	
			}//fin while (lecture du fichier)

			printf("\t%d nouveaux mots enregistrés\n\t%d mises a jour\n",nouveauxMots, maj) ;
			nouveauxMots = 0 ;
			maj = 0 ;
		}//fin for (parcours du dossier)

		count = 0 ;
	}//fin for (traitement des dossiers)

	printf("\n***********************\n"
		   "*Selection des winners*\n"
		   "***********************\n") ;

	printf("\tinitialisation du tableau\n") ;

	winners = calloc(nb_winners, sizeof(Mot)) ;

	if(winners == NULL)
	{
		perror("main : calloc ") ;
		exit(errno) ;
	}

	for(i = 0 ; i < nb_winners ; i++)
		winners[i] = init_mot() ;

	/* Pour chaque entrée du dictionnaire */
	for(i = 0 ; i < TAILLE ; i++)
	{
		ListeMots* liste ;

		liste = dico->contenu[i] ;
		
		/* Parcourir la liste contenue dans l'entrée */
		while (liste->suivant != NULL)
		{
			/* 
			 * Si un élément ayant un plus gros score que celui ayant le score
			 * le plus faible dans le tableau est trouvé
			 */
			if(liste->element.score > winners[minIndex].score)
			{
				int j = 0 ;

				/* Écraser l'élément du tableau */
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
			printf("\t- ") ; 
			print_mot(winners[i]) ; 
			printf("\n") ;
		}
	}

	printf("\n********************\n"
		     "*generation du XML *\n"
		     "********************\n");

	if(generate_xml(winners, nb_winners, (output_file == NULL ? "words.xml" : output_file)) == 0)
		printf("fichier %s enregistré\n", (output_file == NULL ? "words.xml" : output_file)) ;
	else
		printf("erreur lors de la génération du fichier %s\n", (output_file == NULL ? "words.xml" : output_file)) ;

	return 0 ;
}

