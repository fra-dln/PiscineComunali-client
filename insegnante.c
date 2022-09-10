#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <time.h>

#include "defines.h"

//inserire nella variabile PATH il percorso della cartella del client
#define PATH "C:/Users/checc/Desktop/Progetto BD/PiscineComunali-client/"

char cf[18];

static void report_sett(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call report_sett(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize report settimanale statement\n", false);
	}

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for report_sett\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while getting weekly report.");
	}


	printf("%s", cf);

	dump_result_set(conn, prepared_stmt, "\n\nReport Settimanale:\n");

	mysql_stmt_next_result(prepared_stmt);

	mysql_stmt_close(prepared_stmt);
}

static void report_mens(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call report_mens(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize report mens insertion statement\n", false);
	}

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for report_mens\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while getting monthly report.");
	}


	dump_result_set(conn, prepared_stmt, "\n\nReport Mensile:\n");

	mysql_stmt_next_result(prepared_stmt);

	mysql_stmt_close(prepared_stmt);

}

void run_as_insegnante(MYSQL* conn) {
	char* path;
	int op;

	printf("Migrazione ruolo verso INSEGNANTE\n");
	printf("Codice fiscale: ");
	//getchar();
	fflush(stdout);
	fgets(cf, 18, stdin);
	cf[strlen(cf)-1] = '\0';

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

		printf("1) Report Settimanale\n");
		printf("2) Report mensile\n");
		printf("3) Quit \n\n");

		if (scanf("%d", &op) == EOF) {
			fprintf(stderr, "Unable to read from terminal\n");
			exit(EXIT_FAILURE);
		}		

		switch (op) {
		case 1:
			printf("\033[2J\033[H");
			report_sett(conn);
			break;

		case 2: 
			printf("\033[2J\033[H");
			report_mens(conn);
			break;
		case 3:
			return;
		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
		}
	}
}