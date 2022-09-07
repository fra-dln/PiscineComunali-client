#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <time.h>

#include "defines.h"

//inserire nella variabile PATH il percorso della cartella del client
#define PATH "C:/Users/checc/Desktop/Progetto BD/PiscineComunali-client/"
//Variabile globale per mantenere il nome della piscina presso cui il segretario lavora
char* pool;

void run_as_segreteria(MYSQL* conn) {

	char* path;
	int op;

	pool = (char*)malloc(sizeof(char) * 128);
	if (pool == NULL) {
		fprintf(stderr, "unable to allocate space for pool\n");
		exit(EXIT_FAILURE);
	}

	printf("Migrazione ruolo verso SEGRETERIA\n");
	printf("Presso quale piscina ti trovi: ");
	getchar();
	fflush(stdout);
	fgets(pool, 128, stdin);
	printf("\nAvvio operazioni disponibili per la piscina %s\n", pool);
	fflush(stdout);

	path = (char*)malloc(sizeof(char) * 256);
	if (path == NULL) {
		fprintf(stderr, "unable to allocate space for path\n");
		exit(EXIT_FAILURE);
	}

	strcpy(path, PATH);
	strcat(path, "users/segreteria.json");

	if (!parse_config(path, &conf)) {
		fprintf(stderr, "Unable to load segreteria configuration\n");
		exit(EXIT_FAILURE);
	}

	if (mysql_change_user(conn, conf.db_username, conf.db_password, conf.database)) {
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}

	while (true) {
		printf("\n**** Benvenuto! Cosa posso fare per te?****\n");

		/*print di tutte le operazioni che la segreteria può fare*/

		if (scanf("%d", &op) == EOF) {
			fprintf(stderr, "Unable to read from terminal\n");
			exit(EXIT_FAILURE);
		}

		switch (op) {
			case 1:
				/*tutti i vari case*/
			default:
				fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
				abort();
		}
	}



}