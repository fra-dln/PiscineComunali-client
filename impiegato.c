#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <time.h>

#include "defines.h"

//inserire nella variabile PATH il percorso della cartella del client
#define PATH "C:/Users/checc/Desktop/Progetto BD/PiscineComunali-client/"

//TODO: aggiungere operazioni piscine e corpo insegnante



void run_as_impiegato(MYSQL* conn) {
	char* path;
	int op;

	printf("Migrazione ruolo verso IMPIEGATO\n");
	fflush(stdout);

	path = (char*)malloc(sizeof(char) * 256);
	if (path == NULL) {
		fprintf(stderr, "unable to allocate space for path\n");
		exit(EXIT_FAILURE);
	}

	strcpy(path, PATH);
	strcat(path, "users/impiegato.json");

	if (!parse_config(path, &conf)) {
		fprintf(stderr, "Unable to load impiegato configuration\n");
		exit(EXIT_FAILURE);
	}

	if (mysql_change_user(conn, conf.db_username, conf.db_password, conf.database)) {
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}

	while (true) {
		printf("\n**** Benvenuto! Cosa posso fare per te?****\n");

		printf("1) Aggiungi impiego\n");
		printf("2) Aggiungi insegnante\n");
		printf("3) Assegna un insegnante ad un corso\n");
		printf("4) Aggiungi una nuova piscina\n");
		printf("5) Aggiungi una qualifica per un insegnante\n");
		printf("6) Rimuovi insegnante da un corso\n");
		printf("7) Imposta fine impiego\n");
		printf("8) Ottieni informazioni insegnante\n");
		printf("9) Ottieni impieghi passati di un insegnante\n");
		printf("10) Ottieni tutti gli insegnanti che lavorano in una stessa piscina\n");
		printf("11) Ottieni informazioni piscina\n");
		printf("12) Modifica informazioni piscina\n");
		printf("13) Quit\n");
		
		if (scanf("%d", &op) == EOF) {
			fprintf(stderr, "Unable to read from terminal\n");
			exit(EXIT_FAILURE);
		}

		switch (op) {
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
			/*tutti i vari case*/
		case 13:
			return;
		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
		}
	}

}