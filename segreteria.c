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


static void add_contatto(MYSQL* conn, char type[10], char cf[18]) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];

	memset(param, 0, sizeof(param));
	char op;
	char options[3] = { '1','2','3' };
	char recapito[75];
	char CFF[18];
	char string[10];


	if (type == "corsista") {
		if (!setup_prepared_stmt(&prepared_stmt, "call add_contatto_corsista(?,?,?)", conn)) {
			finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize add_contatto_corsista statement\n", false);
		}
	}
	else {
		if (!setup_prepared_stmt(&prepared_stmt, "call add_contatto_libero(?,?,?)", conn)) {
			finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize add_contatto_libero statement\n", false);
		}
	}

	if (strlen(cf) == 0) {
		getchar();
		printf("Codice Fiscale dell'utente: ");
		fgets(CFF, 18, stdin);
		CFF[strlen(CFF) - 1] = '\0';
	}
	else {
		strcpy(CFF, cf);
	}


	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, CFF, strlen(CFF));

	printf("Mezzo di contatto \n");
	printf("\t1)email");
	printf("\t2)cellulare");
	printf("\t3)fisso");
	fflush(stdout);

	op = multiChoice("\tSeleziona mezzo di contatto", options, 3);

	switch (op) {
	case 1:
		strcpy(string, "email");
		set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, string, strlen(string));
	case 2:
		strcpy(string, "cellulare");
		set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, string, strlen(string));
	case 3:
		strcpy(string, "fisso");
		set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, string, strlen(string));
	}

	printf("Recapito: ");
	fgets(recapito, 75, stdin);
	recapito[strlen(recapito) - 1] = '\0';
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, recapito, strlen(recapito));


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_contatto\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while adding contatto.");
	}

	mysql_stmt_close(prepared_stmt);
}

static void add_certificato(MYSQL* conn, char cf[18]) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];

	int eta; char etaStr[4];
	char medico[45];


	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call add_certificato(?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize add_certificato statement\n", false);
	}

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));

	printf("Età dell'utente: ");
	fgets(etaStr, 4, stdin);
	etaStr[strlen(etaStr) - 1] = '\0';
	eta = atoi(etaStr);
	set_binding_param(&param[1], MYSQL_TYPE_LONG, &eta, sizeof(eta));

	printf("Nome e Cognome del medico che certifica: ");
	fflush(stdout);
	fgets(medico, 45, stdin);
	medico[strlen(medico) - 1] = '\0';
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, medico, strlen(medico));


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_certificato\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while add_certificato.");
	}

	mysql_stmt_close(prepared_stmt);

}
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
	fflush(stdout);
	fgets(cf, 18, stdin);
	cf[strlen(cf) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));

	printf("Nome e Cognome: ");
	fgets(name, 47, stdin);
	name[strlen(name) - 1] = '\0';
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, name, strlen(name));

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

	
	
	mysql_stmt_close(prepared_stmt);
	add_contatto(conn, "corsista", cf);
	add_certificato(conn, cf);
	printf("Utente corsista correttemente inserito \n\n");
}


static void add_nuotatore_libero(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[5];
	struct MYSQL_TIME date;

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

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

	printf("Nome e Cognome: ");
	fgets(name, 47, stdin);
	name[strlen(name) - 1] = '\0';
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, name, strlen(name));

	printf("Indirizzo di Residenza: ");
	fgets(address, 47, stdin);
	address[strlen(address) - 1] = '\0';
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, address, strlen(address));

	pool[strlen(pool) - 1] = '\0';
	set_binding_param(&param[3], MYSQL_TYPE_VAR_STRING, pool, strlen(pool));

	date.day = tm.tm_mday; 
	date.month = tm.tm_mon+1; 
	date.year = tm.tm_year+1900; 

	param[4].buffer_type = MYSQL_TYPE_DATE;
	param[4].buffer = &date;
	param[4].buffer_length = sizeof(MYSQL_TIME);



	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_libero\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while adding user libero.");
	}

	mysql_stmt_close(prepared_stmt);

	add_contatto(conn, "libero", cf);

	printf("\nNuotatore libero correttamente inserito\n");
}

/*
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

	printf("Minimo numero di partecipanti: ");
	fgets(minStr, 3, stdin);
	minStr[strlen(minStr) - 1] = '\0';

	int min = atoi(minStr);
	set_binding_param(&param[1], MYSQL_TYPE_LONG, &min, sizeof(min));
	

	printf("Massimo numero di partecipanti: ");
	fgets(maxStr, 3, stdin);
	maxStr[strlen(maxStr) - 1] = '\0';

	int max = atoi(maxStr);
	set_binding_param(&param[2], MYSQL_TYPE_LONG, &max, sizeof(max));

	printf("Costo per lezione: ");
	fgets(costoStr, 3, stdin);
	costoStr[strlen(costoStr) - 1] = '\0';

	int costo = atoi(costoStr);
	set_binding_param(&param[3], MYSQL_TYPE_LONG, &costo, sizeof(costo));

	printf("Nome dell'attivita': ");
	fgets(attivita, 151, stdin);
	attivita[strlen(attivita) - 1] = '\0';

	set_binding_param(&param[4], MYSQL_TYPE_VAR_STRING, attivita, strlen(attivita));

	//modifica add corso per aggiungere i giorni o aggiungi una query

}*/

static void del_contatto(MYSQL* conn, char type[10]) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];
	char recapito[75];

	memset(param, 0, sizeof(param));

	if (type == "corsista") {
		if (!setup_prepared_stmt(&prepared_stmt, "call del_contatto_corsista(?)", conn)) {
			finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize del_contatto_corsista statement\n", false);
		}
	}
	else {
		if (!setup_prepared_stmt(&prepared_stmt, "call del_contatto_libero(?)", conn)) {
			finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize del_contatto_libero statement\n", false);
		}
	}
	getchar();
	printf("Recapito da cancellare: ");
	fgets(recapito, 75, stdin);
	recapito[strlen(recapito) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, recapito, strlen(recapito));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_contatto\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while adding contatto.");
	}
	
	printf("contatto cancellato correttamente \n\n");
	mysql_stmt_close(prepared_stmt);

}

static void del_user(MYSQL* conn, char type[10]) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];
	char cf[18];

	memset(param, 0, sizeof(param));

	if (type == "corsista") {
		if (!setup_prepared_stmt(&prepared_stmt, "call del_corsista(?)", conn)) {
			finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize del_contatto_corsista statement\n", false);
		}
	}
	else {
		if (!setup_prepared_stmt(&prepared_stmt, "call del_libero(?)", conn)) {
			finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize del_contatto_libero statement\n", false);
		}
	}

	getchar();
	printf("Codice Fiscale dell'utente da rimuovere dal sistema: ");
	fgets(cf, 18, stdin);
	cf[strlen(cf) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for del_user\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while del user.");
	}

	printf("utente cancellato correttamente \n\n");
	mysql_stmt_close(prepared_stmt);
}


static void del_corso(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];
	char idStr[5]; int id;

	memset(param, 0, sizeof(param));
	
	if (!setup_prepared_stmt(&prepared_stmt, "call del_corso(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize del_contatto_corsista statement\n", false);
	}
	getchar();
	printf("ID corso da rimuovere: ");
	fgets(idStr, 5, stdin);
	idStr[strlen(idStr) - 1] = '\0';
	id = atoi(idStr);


	set_binding_param(&param[0], MYSQL_TYPE_LONG, &id, sizeof(id));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for del_corso\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while del_corso.");
	}

	printf("corso cancellato correttamente \n\n");
	mysql_stmt_close(prepared_stmt);
}

static void del_iscrizione(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[2];
	char idStr[5]; char cf[18];
	int id;

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call del_iscrizione(?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize del_contatto_corsista statement\n", false);
	}

	getchar();
	printf("Codice Fiscale dell'utente: ");
	fgets(cf, 18, stdin);
	cf[strlen(cf) - 1] = '\0';
	
	printf("ID corso da rimuovere: ");
	fgets(idStr, 5, stdin);
	idStr[strlen(idStr) - 1] = '\0';
	id = atoi(idStr);

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));
	set_binding_param(&param[1], MYSQL_TYPE_LONG, &id, sizeof(id));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for del_corso\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while del_corso.");
	}

	printf("iscrizione cancellata correttamente \n\n");
	mysql_stmt_close(prepared_stmt);
}

static void sign_corsista(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];
	char idStr[5]; char cf[18];
	int id;

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call sign_corsista(?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize del_contatto_corsista statement\n", false);
	}
	getchar();
	printf("Codice Fiscale dell'utente: ");
	fgets(cf, 18, stdin);
	cf[strlen(cf) - 1] = '\0';

	printf("ID corso: ");
	fgets(idStr, 5, stdin);
	idStr[strlen(idStr) - 1] = '\0';
	id = atoi(idStr);

	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, pool, strlen(pool));
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));
	set_binding_param(&param[0], MYSQL_TYPE_LONG, &id, sizeof(id));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for sign corsista\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while sign corsista.");
	}

	printf("iscrizione effettuata correttamente \n\n");
	mysql_stmt_close(prepared_stmt);

}

static void get_user(MYSQL* conn, char type[10]) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];
	char cf[18];

	memset(param, 0, sizeof(param));

	if (type == "corsista") {
		if (!setup_prepared_stmt(&prepared_stmt, "call get_corsista(?)", conn)) {
			finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize get_corsista statement\n", false);
		}
	}
	else {
		if (!setup_prepared_stmt(&prepared_stmt, "call get_libero(?)", conn)) {
			finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize get_libero statement\n", false);
		}
	}
	getchar();
	printf("Codice Fiscale dell'utente: ");
	fgets(cf, 18, stdin);
	cf[strlen(cf) - 1] = '\0';


	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for get_user\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while get_user.");
	}

	dump_result_set(conn, prepared_stmt, "\n\nInfo utente:\n");

	mysql_stmt_next_result(prepared_stmt);

	mysql_stmt_close(prepared_stmt);
}

static void get_corso(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[2];
	char idStr[5]; char pool[50];
	int id;

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call get_corso(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize get_corso statement\n", false);
	}
	getchar();
	printf("ID corso: ");
	fgets(idStr, 5, stdin);
	idStr[strlen(idStr) - 1] = '\0';
	id = atoi(idStr);

	set_binding_param(&param[0], MYSQL_TYPE_LONG, &id, sizeof(id));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for get_corso\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while getting corso info.");
	}


	dump_result_set(conn, prepared_stmt, "\n\nInfo Corso:\n");

	mysql_stmt_next_result(prepared_stmt);

	mysql_stmt_close(prepared_stmt);

}

static void set_ultima_piscina(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[2];
	char cf[18];
	
	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call set_ultima_piscina(?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize set_ultima_piscina statement\n", false);
	}
	getchar();
	printf("Codice Fiscale dell'utente: ");
	fgets(cf, 18, stdin);
	cf[strlen(cf) - 1] = '\0';

	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, pool, strlen(pool));
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for set_ultima_piscina\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while set_ultima_piscina.");
	}

	mysql_stmt_close(prepared_stmt);

}

static void renew_certificato(MYSQL* conn) {

	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];

	int eta; char etaStr[4];
	char medico[45]; char cf[18];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call add_certificato(?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize add_certificato statement\n", false);
	}
	getchar();
	printf("Codice Fiscale dell'utente: ");
	fgets(cf, 16, stdin);
	cf[strlen(cf) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));

	getchar();
	printf("Età dell'utente: ");
	fgets(etaStr, 4, stdin);
	etaStr[strlen(etaStr) - 1] = '\0';
	eta = atoi(etaStr);
	set_binding_param(&param[1], MYSQL_TYPE_LONG, &eta, sizeof(eta));

	getchar();
	printf("Nome e Cognome del medico che certifica: ");
	fgets(medico, 45, stdin);
	medico[strlen(medico) - 1] = '\0';
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, medico, strlen(medico));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_certificato\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while add_certificato.");
	}

	mysql_stmt_close(prepared_stmt);

}


void run_as_segreteria(MYSQL* conn) {

	char* path;
	int op;

	printf("Migrazione ruolo verso SEGRETERIA\n");
	printf("Presso quale piscina ti trovi: ");
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
		printf("17) Imposta ultima visita nuotatore libero\n");
		printf("18) Rinnovo certificato\n");
		printf("19) Quit\n\n");

		if (scanf("%d", &op) == EOF) {
			fprintf(stderr, "Unable to read from terminal\n");
			exit(EXIT_FAILURE);
		}
		fflush(stdout);
		switch (op) {
			case 1:
				add_corsista(conn); break;
			case 2:
				add_nuotatore_libero(conn); break;
			case 3:
				printf("YET TO BE IMPLEMENTED\n\n\n\n"); break;
				/****************************************/
			case 4:
				add_contatto(conn,"corsista",""); break;
			case 5:
				add_contatto(conn, "libero", ""); break;
			case 6:
				del_contatto(conn, "corsista"); break;
			case 7:
				del_contatto(conn, "libero"); break;
			case 8:
				del_user(conn, "corsista"); break;
			case 9:
				del_user(conn, "libero"); break;
			case 10:
				del_corso(conn); break;
			case 11:
				del_iscrizione(conn); break;
			case 12:
				sign_corsista(conn); break;
			case 13:
				get_user(conn, "corsista"); break;
			case 14:
				get_user(conn, "libero"); break;
			case 15:
				get_corso(conn); break;
			case 16:
				printf("YET TO BE IMPLEMENTED\n\n\n\n"); break;
				/******************************************************/
			case 17:
				set_ultima_piscina(conn); break;
			case 18:
				renew_certificato(conn); break;
			case 19:
				return;
			default:
				fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
				abort();
		}
		
	}



}