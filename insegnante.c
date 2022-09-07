#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <time.h>

#include "defines.h"

//inserire nella variabile PATH il percorso della cartella del client
#define PATH "C:/Users/checc/Desktop/Progetto BD/PiscineComunali-client/"

char* cf;

void run_as_insegnante(MYSQL* conn) {
	char* path;
	int op;

	cf = (char*)malloc(sizeof(char) * 15);
	if (cf == NULL) {
		fprintf(stderr, "unable to allocate space for cf\n");
		exit(EXIT_FAILURE);
	}

	printf("Migrazione ruolo verso INSEGNANTE\n");
	printf("Codice fiscale: ");
	getchar();
	fflush(stdout);
	fgets(cf, 15, stdin);

	path = (char*)malloc(sizeof(char) * 256);
	if (path == NULL) {
		fprintf(stderr, "unable to allocate space for path\n");
		exit(EXIT_FAILURE);
	}

	strcpy(path, PATH);
	strcat(path, "users/insegnante.json");

	if (!parse_config(path, &conf)) {
		fprintf(stderr, "Unable to load insegnante configuration\n");
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