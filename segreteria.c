#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <time.h>

#include "defines.h"

//inserire nella variabile PATH il percorso della cartella del client
#define PATH "C:/Users/checc/Desktop/Progetto BD/PiscineComunali-client/"
//Variabile globale per mantenere il nome della piscina presso cui il segretario lavora
char pool[52];

//TODO: inserire operazioni riguardo gli utenti e riguardo i corsi per le piscine le quali lavorano

static void add_corsista(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];

	char cf[18]; char name[47]; char address[82];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call add_corsista(?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize add corsista statement\n", false);
	}

	getchar();
	printf("Codice Fiscale: ");
	fgets(cf, 18, stdin);
	cf[strlen(cf) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));

	getchar();
	printf("Nome e Cognome: ");
	fgets(name, 47, stdin);
	name[strlen(name) - 1] = '\0';
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, name, strlen(name));

	getchar();
	printf("Indirizzo di Residenza: ");
	fgets(address, 47, stdin);
	address[strlen(address) - 1] = '\0';
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, address, strlen(address));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_corsista\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while adding user corsista.");
	}
	else {
		printf("\nCorsista correttamente inserito\n");
	}

	// aggiungi contatti 
	//certificato???



	mysql_stmt_close(prepared_stmt);

}

static void add_nuotatore_libero(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[5];

	char cf[18]; char name[47]; char address[82]; 

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call add_libero(?,?,?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize add libero statement\n", false);
	}

	getchar();
	printf("Codice Fiscale: ");
	fgets(cf, 18, stdin);
	cf[strlen(cf) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));

	getchar();
	printf("Nome e Cognome: ");
	fgets(name, 47, stdin);
	name[strlen(name) - 1] = '\0';
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, name, strlen(name));

	getchar();
	printf("Indirizzo di Residenza: ");
	fgets(address, 47, stdin);
	address[strlen(address) - 1] = '\0';
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, address, strlen(address));

	pool[strlen(pool) - 1] = '\0';
	set_binding_param(&param[3], MYSQL_TYPE_VAR_STRING, pool, strlen(pool));

	//binding del dato data corrente come faccio?

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_libero\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while adding user libero.");
	}
	else {
		printf("\nNuotatore libero correttamente inserito\n");
	}


	//aggiungi contatti

	mysql_stmt_close(prepared_stmt);


}

static void add_corso(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[5];

	char minStr[3];
	char maxStr[3];
	char costoStr[5];
	char attivita[152];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call add_corso(?,?,?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize add_corso statement\n", false);
	}

	pool[strlen(pool) - 1] = '\0';
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, pool, strlen(pool));

	getchar();
	printf("Minimo numero di partecipanti: ");
	fgets(minStr, 3, stdin);
	minStr[strlen(minStr) - 1] = '\0';

	int min = atoi(minStr);
	set_binding_param(&param[1], MYSQL_TYPE_LONG, &min, sizeof(min));
	

	getchar();
	printf("Massimo numero di partecipanti: ");
	fgets(maxStr, 3, stdin);
	maxStr[strlen(maxStr) - 1] = '\0';

	int max = atoi(maxStr);
	set_binding_param(&param[2], MYSQL_TYPE_LONG, &max, sizeof(max));

	getchar();
	printf("Costo per lezione: ");
	fgets(costoStr, 3, stdin);
	costoStr[strlen(costoStr) - 1] = '\0';

	int costo = atoi(costoStr);
	set_binding_param(&param[3], MYSQL_TYPE_LONG, &costo, sizeof(costo));

	getchar();
	printf("Nome dell'attivita': ");
	fgets(attivita, 151, stdin);
	attivita[strlen(attivita) - 1] = '\0';

	set_binding_param(&param[4], MYSQL_TYPE_VAR_STRING, attivita, strlen(attivita));

	//modifica add corso per aggiungere i giorni o aggiungi una query



}


void run_as_segreteria(MYSQL* conn) {

	char* path;
	int op;

	printf("Migrazione ruolo verso SEGRETERIA\n");
	printf("Presso quale piscina ti trovi: ");
	getchar();
	fflush(stdout);
	fgets(pool, 52, stdin);
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

		printf("1) Aggiungi corsista\n");
		printf("2) Aggiungi nuotatore libero\n");
		printf("3) Aggiungi corso\n");
		printf("4) Aggiungi contatto corsista\n");
		printf("5) Aggiungi contatto nuotatore libero\n");
		printf("6) Rimuovi contatto corsista\n");
		printf("7) Rimuovi contatto nuotatore libero\n");
		printf("8) Rimuovi corsista\n");
		printf("9) Rimuovi nuotatore libero\n");
		printf("10) Rimuovi Corso\n");
		printf("11) Cancella iscrizione a un corso\n");
		printf("12) Iscrivi corsista a un corso\n");
		printf("13) Ottieni informazioni corsista\n");
		printf("14) Ottieni informazioni nuotatore libero\n");
		printf("15) Ottieni informazioni corso\n");
		printf("16) Modifica corso\n");
		printf("17) Modifica piscina\n");
		printf("18) Imposta ultima visita nuotatore libero\n");
		printf("19) Rinnovo certificato\n");
		printf("20) Quit\n\n");

		if (scanf("%d", &op) == EOF) {
			fprintf(stderr, "Unable to read from terminal\n");
			exit(EXIT_FAILURE);
		}
	
		switch (op) {
			case 1:
				/*tutti i vari case*/
			case 2:
				/*tutti i vari case*/
			case 3:
				/*tutti i vari case*/
			case 4:
				
			case 5:
				/*tutti i vari case*/
			case 6:
				/*tutti i vari case*/
			case 7:
				/*tutti i vari case*/
			case 8:
				/*tutti i vari case*/
			case 9:
				/*tutti i vari case*/
			case 10:
				/*tutti i vari case*/
			case 11:

			case 12:

			case 13:

			case 14:

			case 15:

			case 17:

			case 18:

			case 19:

			case 20: 
				return;
			default:
				fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
				abort();
		}
		
	}



}