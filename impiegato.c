#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <time.h>

#include "defines.h"

//inserire nella variabile PATH il percorso della cartella del client
#define PATH "C:/Users/checc/Desktop/Progetto BD/PiscineComunali-client/"

static void add_impiego(MYSQL *conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[5];

	char pool[52];
	char cf[18];
	char type[12];

	int giorno, mese, anno;
	char giornoStr[3]; char meseStr[3]; char annoStr[5];

	struct MYSQL_TIME dataInizio;
	struct MYSQL_TIME fake;
	struct MYSQL_TIME dataFine;

	char op;
	char options[2] = { '1','2' };

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call add_impiego(?,?,?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize report settimanale statement\n", false);
	}
	printf("*** Aggiungi un nuovo impiego per un insegnante***\n\n");
	fflush(stdout);

	getchar();
	printf("In quale piscina? ");
	fgets(pool, 52, stdin);
	pool[strlen(pool) - 1] = '\0';

	printf("Codice Fiscale insegnante: ");
	fgets(cf, 18, stdin);
	cf[strlen(cf) - 1] = '\0';

	printf("Che tipo di impiego e'?\n");
	printf("\t1)presente");
	printf("\t2)passato\n\n");
	fflush(stdout);

	op = multiChoice("Seleziona tipo d'impiego", options, 2);

	switch (op) {
	case '1':
		strcpy(type, "presente");
		fake.day = 1;
		fake.month = 1;
		fake.year = 1900;
		break;

	case '2':
		strcpy(type, "passato");
		break;

	default:
		fprintf(stderr, "\nInvalid condition at %s:%d\n", __FILE__, __LINE__);
		abort();
	}

	printf("Quando ha avuto inizio l'impiego? (giorno mese e anno vanno espressi in numeri GG MM YYYY)\n");
	printf("Giorno: ");
	fgets(giornoStr, 3, stdin);
	giorno = atoi(giornoStr);

	getchar();
	printf("Mese: "); 
	fgets(meseStr, 3, stdin);
	mese = atoi(meseStr);

	getchar();
	printf("Anno: "); 
	fgets(annoStr, 5, stdin);
	anno = atoi(annoStr);

	dataInizio.day = giorno;
	dataInizio.month = mese;
	dataInizio.year = anno;

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, pool, strlen(pool));
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));
	param[2].buffer_type = MYSQL_TYPE_DATE;
	param[2].buffer = &dataInizio;
	param[2].buffer_length = sizeof(MYSQL_TIME);

	printf("before if\n");
	if (op=='2') {

		printf("\n\n dentro if \n\n");
		printf("Quando ha avuto fine l'impiego? (giorno mese e anno vanno espressi in numeri GG MM YYYY)\n");

		getchar();
		printf("Giorno: ");
		fgets(giornoStr, 3, stdin);
		giorno = atoi(giornoStr);
		getchar();
		printf("Mese: ");
		fgets(meseStr, 3, stdin);
		mese = atoi(meseStr);
		getchar();
		printf("Anno: ");
		fgets(annoStr, 5, stdin);
		anno = atoi(annoStr);

		dataFine.day = giorno;
		dataFine.month = mese;
		dataFine.year = anno;

		param[3].buffer_type = MYSQL_TYPE_DATE;
		param[3].buffer = &dataFine;
		param[3].buffer_length = sizeof(MYSQL_TIME);

	}
	else {

		printf("dentro else\n\n");
		param[3].buffer_type = MYSQL_TYPE_DATE;
		param[3].buffer = &fake;
		param[3].buffer_length = sizeof(MYSQL_TIME);
	}
	printf("After if\n");

	set_binding_param(&param[4], MYSQL_TYPE_VAR_STRING, type, strlen(type));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_impiego\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while getting adding impiego.");
	}

	printf("\n\nImpiego aggiunto correttamente!\n\n");
	fflush(stdout);

	mysql_stmt_close(prepared_stmt);

}

static void add_insegnante(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];

	char cf[18];
	char name[45];
	char cell[10];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call add_insegnante(?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize add insegnante statement\n", false);
	}
	printf("Aggiunta insegnante...\n\n");
	
	getchar();
	printf("Codice Fiscale: ");
	fflush(stdout);
	fgets(cf, 18, stdin);
	cf[strlen(cf) - 1] = '\0';

	printf("Nome e Cognome: ");
	fflush(stdout);
	fgets(name, 45, stdin);
	name[strlen(name) - 1] = '\0';
	
	printf("Numero di cellulare: ");
	fflush(stdout);
	fgets(cell, 10, stdin);
	cell[strlen(cell) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, name, strlen(name));
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, cell, strlen(cell));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_insegnante\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while getting adding insegnante.");
	}

	printf("\n\nInsegnante aggiunto correttamente!\n\n");
	fflush(stdout);

	mysql_stmt_close(prepared_stmt);

}

static void add_insegnante_corso(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];

	char cf[18];
	char pool[52];
	char idStr[5];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call add_insegnante_corso(?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize add insegnante corso statement\n", false);
	}
	printf("Assegnazione insegnante...\n\n");

	getchar();
	printf("Codice Fiscale: ");
	fflush(stdout);
	fgets(cf, 18, stdin);
	cf[strlen(cf) - 1] = '\0';

	printf("In quale piscina si tiene il corso? ");
	fgets(pool, 52, stdin);
	pool[strlen(pool) - 1] = '\0';

	printf("ID corso:  ");
	fgets(idStr, 5, stdin);
	int id = atoi(idStr);

	set_binding_param(&param[0], MYSQL_TYPE_LONG, &id, sizeof(id));
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, pool, strlen(pool));
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_insegnante_corso\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while getting adding insegnante corso.");
	}

	printf("\n\nInsegnante assegnato correttamente!\n\n");
	fflush(stdout);

	mysql_stmt_close(prepared_stmt);
}

static void add_piscina(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[6];

	char pool[50];
	char resp[45];
	char indirizzo[80];

	int giorno, mese, anno;
	char giornoStr[3]; char meseStr[3]; char annoStr[5];
	int type;

	struct MYSQL_TIME dataInizio;
	struct MYSQL_TIME dataFine;

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call add_piscina(?,?,?,?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize report settimanale statement\n", false);
	}
	printf("*** Creazione nuova piscina ***\n\n");

	getchar();
	printf("Nome piscina: ");
	fflush(stdout);
	fgets(pool, 50, stdin);
	pool[strlen(pool) - 1] = '\0';

	printf("Nome e cognome responsabile: ");
	fflush(stdout);
	fgets(resp, 45, stdin);
	resp[strlen(resp) - 1] = '\0';

	printf("Indirizzo piscina: ");
	fflush(stdout);
	fgets(indirizzo, 18, stdin);
	indirizzo[strlen(indirizzo) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, pool, strlen(pool));
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, resp, strlen(resp));
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, indirizzo, strlen(indirizzo));

	char op;
	char options[2] = { '1','2' };

	printf("La piscina ha una vasca all'aperto? (rispondere con 1 o 2)\n");
	printf("\t1)YES");
	printf("\t2)NO");
	fflush(stdout);

	op = multiChoice("\tSeleziona tipo piscina", options, 2);


	switch (op) {
	case '1':
		type = 1;
		set_binding_param(&param[3], MYSQL_TYPE_LONG, &type, sizeof(type));

		printf("Quando apre la vasca all'aperto? (giorno mese e anno vanno espressi in numeri)\n");

		printf("Giorno: ");
		fgets(giornoStr, 3, stdin);
		giorno = atoi(giornoStr);

		getchar();
		printf("Mese: ");
		fgets(meseStr, 3, stdin);
		mese = atoi(meseStr);

		getchar();
		printf("Anno: ");
		fgets(annoStr, 5, stdin);
		anno = atoi(annoStr);
		anno = atoi(annoStr);

		dataInizio.day = giorno;
		dataInizio.month = mese;
		dataInizio.year = anno;

		param[4].buffer_type = MYSQL_TYPE_DATE;
		param[4].buffer = &dataInizio;
		param[4].buffer_length = sizeof(MYSQL_TIME);

		printf("Quando chiude la vasca all'aperto? (giorno mese e anno vanno espressi in numeri)\n");

		getchar();
		printf("Giorno: ");
		fgets(giornoStr, 3, stdin);
		giorno = atoi(giornoStr);

		getchar();
		printf("Mese: ");
		fgets(meseStr, 3, stdin);
		mese = atoi(meseStr);

		getchar();
		printf("Anno: ");
		fgets(annoStr, 5, stdin);
		anno = atoi(annoStr);

		dataFine.day = giorno;
		dataFine.month = mese;
		dataFine.year = anno;

		param[5].buffer_type = MYSQL_TYPE_DATE;
		param[5].buffer = &dataFine;
		param[5].buffer_length = sizeof(MYSQL_TIME);

		break;

	case '2':
		type = 0;

		set_binding_param(&param[3], MYSQL_TYPE_LONG, &type, sizeof(type));

		dataInizio.day = 1;
		dataInizio.month = 1;
		dataInizio.year = 1900;

		param[4].buffer_type = MYSQL_TYPE_DATE;
		param[4].buffer = &dataInizio;
		param[4].buffer_length = sizeof(MYSQL_TIME);

		dataFine.day = 1;
		dataFine.month = 1;
		dataFine.year = 1900;

		param[5].buffer_type = MYSQL_TYPE_DATE;
		param[5].buffer = &dataFine;
		param[5].buffer_length = sizeof(MYSQL_TIME);

		break;

	default:
		fprintf(stderr, "\nInvalid condition at %s:%d\n", __FILE__, __LINE__);
		abort();
	}

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_piscina\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while getting adding piscina.");
	}

	printf("\n\nPiscina aggiunta correttamente!\n\n");
	fflush(stdout);

	mysql_stmt_close(prepared_stmt);

}

static void add_qualifica(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[2];

	char cf[18];
	char titolo[50];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call add_qualifica(?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize report settimanale statement\n", false);
	}
	printf("*** Aggiungi qualifica insegnante ***\n\n");

	getchar();
	printf("Codice fiscale insegnante: ");
	fflush(stdout);
	fgets(cf, 18, stdin);
	cf[strlen(cf) - 1] = '\0';

	printf("Titolo qualifica: ");
	fflush(stdout);
	fgets(titolo, 50, stdin);
	titolo[strlen(titolo) - 1] = '\0';

	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, titolo, strlen(titolo));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for add_qualifica\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while adding qualifica.");
	}

	printf("\n\nQualifica aggiunta correttamente!\n\n");
	fflush(stdout);

	mysql_stmt_close(prepared_stmt);


}

static void del_insegnante_corso(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[2];

	char cf[18];
	char idStr[5];

	int id;

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call del_insegnante_corso(?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize del insegnante corso\n", false);
	}

	printf("*** Rimozione insegnante da un corso ***\n\n");
	getchar();
	printf("Codice fiscale insegnante: ");
	fflush(stdout);
	fgets(cf, 18, stdin);
	cf[strlen(cf) - 1] = '\0';
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));
	

	printf("ID corso: ");
	fflush(stdout);
	fgets(idStr, 5, stdin);
	idStr[strlen(idStr) - 1] = '\0';
	id = atoi(idStr);
	set_binding_param(&param[1], MYSQL_TYPE_LONG, &id, sizeof(id));

	printf("%s\t\t%d", cf, id);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for del_insegnantecorso\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while removing insegnante corso.");
	}

	printf("\n\nInsegnante rimosso correttamente dal corso %d!\n\n",id);
	fflush(stdout);

	mysql_stmt_close(prepared_stmt);


}


static void fine_impiego(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[4];

	char cf[18];
	char pool[52];
	int giorno, mese, anno;
	char giornoStr[3]; char meseStr[3]; char annoStr[5];

	struct MYSQL_TIME dataInizio;
	struct MYSQL_TIME dataFine;

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call fine_impiego(?,?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize del insegnante corso\n", false);
	}
	printf("***** IMPOSTA FINE IMPIEGO *****\n\n");

	printf("Piscina dove l'insegnante è impiegato:  ");
	fgets(pool, 52, stdin);
	pool[strlen(pool) - 1] = '\0';

	printf("Codice fiscale insegnante: ");
	fflush(stdout);
	fgets(cf, 16, stdin);
	cf[strlen(cf) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, pool, strlen(pool));
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));

	printf("Quando ha avuto inizio l'impiego? (giorno mese e anno vanno espressi in numeri)\n");
	printf("Giorno: ");
	fgets(giornoStr, 3, stdin);
	giorno = atoi(giornoStr);

	printf("Mese: ");
	fgets(meseStr, 3, stdin);
	mese = atoi(meseStr);

	printf("Anno: ");
	fgets(annoStr, 5, stdin);
	anno = atoi(annoStr);

	dataInizio.day = giorno;
	dataInizio.month = mese;
	dataInizio.year = anno;

	param[2].buffer_type = MYSQL_TYPE_DATE;
	param[2].buffer = &dataInizio;
	param[2].buffer_length = sizeof(MYSQL_TIME);

	printf("Quando ha fine l'impiego? (giorno mese e anno vanno espressi in numeri)\n");
	printf("Giorno: ");
	fgets(giornoStr, 3, stdin);
	giorno = atoi(giornoStr);

	printf("Mese: ");
	fgets(meseStr, 3, stdin);
	mese = atoi(meseStr);

	printf("Anno: ");
	fgets(annoStr, 5, stdin);
	anno = atoi(annoStr);

	dataFine.day = giorno;
	dataFine.month = mese;
	dataFine.year = anno;

	param[3].buffer_type = MYSQL_TYPE_DATE;
	param[3].buffer = &dataFine;
	param[3].buffer_length = sizeof(MYSQL_TIME);

	
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for fine impiego\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while fine impiego.");
	}

	printf("\n\nFine impiego eseguito correttamente!\n\n");
	fflush(stdout);

	mysql_stmt_close(prepared_stmt);

}


static void get_insegnante(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];

	char cf[18];
	printf("******get info insegnante*****\n\n");
	
	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call get_insegnante(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize get insegnante statement\n", false);
	}
	getchar();
	printf("Codice fiscale insegnante: ");
	fflush(stdout);
	fgets(cf, 18, stdin);
	cf[strlen(cf) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for get_insegnante\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while getting insegnante info.");
	}


	dump_result_set(conn, prepared_stmt, "\n\nInformazioni richieste riguardo l'insegnante:\n");

	mysql_stmt_next_result(prepared_stmt);

	mysql_stmt_close(prepared_stmt);
}

static void get_impieghi_passati(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];

	char cf[18];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call get_impieghi_passati(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize get_impieghi_passati statement\n", false);
	}

	printf("******get impieghi passati insegnante*****\n\n");
	getchar();
	printf("Codice fiscale insegnante: ");
	fflush(stdout);
	fgets(cf, 18, stdin);
	cf[strlen(cf) - 1] = '\0';


	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cf, strlen(cf));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for get_impieghi_passati\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while get_impieghi_passati.");
	}


	dump_result_set(conn, prepared_stmt, "\n\nImpieghi passati:\n");

	mysql_stmt_next_result(prepared_stmt);

	mysql_stmt_close(prepared_stmt);
}

static void get_insegnante_piscina(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];

	char pool[52];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call get_insegnante_piscina(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize get_insegnante_piscina statement\n", false);
	}

	printf("******insegnanti che lavorano presso una piscina*****\n\n");
	getchar();
	printf("Nome piscina: ");
	fflush(stdout);
	fgets(pool, 50, stdin);
	pool[strlen(pool) - 1] = '\0';


	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, pool, strlen(pool));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for get_insegnante_piscina\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while get_insegnante_piscina.");
	}


	dump_result_set(conn, prepared_stmt, "\n\nInsegnanti che lavorano presso la piscina richiesta:\n");

	mysql_stmt_next_result(prepared_stmt);

	mysql_stmt_close(prepared_stmt);
}


static void get_piscina(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[1];

	char pool[52];

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call get_piscina(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize get_piscina statement\n", false);
	}

	printf("******Informazioni piscina*****\n\n");
	getchar();
	printf("Nome piscina: ");
	fflush(stdout);
	fgets(pool, 50, stdin);
	pool[strlen(pool) - 1] = '\0';


	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, pool, strlen(pool));

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for get_piscina\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while get_piscina.");
	}

	dump_result_set(conn, prepared_stmt, "\n\nInfo piscina: \n");

	mysql_stmt_next_result(prepared_stmt);

	mysql_stmt_close(prepared_stmt);
}

static void mod_piscina(MYSQL* conn) {
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[6];

	char pool[52];
	char resp[45];
	char indirizzo[80];

	int giorno, mese, anno;
	char giornoStr[3]; char meseStr[3]; char annoStr[5];
	int type;

	struct MYSQL_TIME dataInizio;
	struct MYSQL_TIME dataFine;

	memset(param, 0, sizeof(param));

	if (!setup_prepared_stmt(&prepared_stmt, "call mod_piscina(?,?,?,?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "\nUnable to initialize mod_piscina statement\n", false);
	}
	printf("*** Modifica info piscina ***\n\n");
	getchar();
	printf("Nome piscina: ");
	fflush(stdout);
	fgets(pool, 52, stdin);
	pool[strlen(pool) - 1] = '\0';

	printf("Nome e cognome responsabile: ");
	fflush(stdout);
	fgets(resp, 45, stdin);
	resp[strlen(resp) - 1] = '\0';

	printf("Indirizzo piscina: ");
	fflush(stdout);
	fgets(indirizzo, 80, stdin);
	indirizzo[strlen(indirizzo) - 1] = '\0';

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, pool, strlen(pool));
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, resp, strlen(resp));
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, indirizzo, strlen(indirizzo));

	char op;
	char options[2] = { '1','2' };

	printf("La piscina ha una vasca all'aperto? (rispondere con 1 o 2)\n");
	printf("\t1)YES");
	printf("\t2)NO");
	fflush(stdout);

	op = multiChoice("Seleziona tipo piscina", options, 2);

	switch (op) {
	case '1':
		type = 1;
		set_binding_param(&param[3], MYSQL_TYPE_LONG, &type, sizeof(type));
		getchar();
		printf("Quando apre la vasca all'aperto? (giorno mese e anno vanno espressi in numeri)\n");
		printf("Giorno: ");
		fgets(giornoStr, 3, stdin);
		giorno = atoi(giornoStr);
		getchar();
		printf("Mese: ");
		fgets(meseStr, 3, stdin);
		mese = atoi(meseStr);
		getchar();
		printf("Anno: ");
		fgets(annoStr, 5, stdin);
		anno = atoi(annoStr);

		dataInizio.day = giorno;
		dataInizio.month = mese;
		dataInizio.year = anno;

		param[4].buffer_type = MYSQL_TYPE_DATE;
		param[4].buffer = &dataInizio;
		param[4].buffer_length = sizeof(MYSQL_TIME);
		getchar();
		printf("Quando chiude la vasca all'aperto? (giorno mese e anno vanno espressi in numeri)\n");
		printf("Giorno: ");
		fgets(giornoStr, 3, stdin);
		giorno = atoi(giornoStr);
		getchar();
		printf("Mese: ");
		fgets(meseStr, 3, stdin);
		mese = atoi(meseStr);
		getchar();
		printf("Anno: ");
		fgets(annoStr, 5, stdin);
		anno = atoi(annoStr);

		dataFine.day = giorno;
		dataFine.month = mese;
		dataFine.year = anno;

		param[5].buffer_type = MYSQL_TYPE_DATE;
		param[5].buffer = &dataFine;
		param[5].buffer_length = sizeof(MYSQL_TIME);

		break;

	case '2':
		type = 0;

		set_binding_param(&param[3], MYSQL_TYPE_TINY, type, sizeof(type));

		dataInizio.day = 1;
		dataInizio.month = 1;
		dataInizio.year = 1900;

		param[4].buffer_type = MYSQL_TYPE_DATE;
		param[4].buffer = &dataInizio;
		param[4].buffer_length = sizeof(MYSQL_TIME);

		dataFine.day = 1;
		dataFine.month = 1;
		dataFine.year = 1900;

		param[5].buffer_type = MYSQL_TYPE_DATE;
		param[5].buffer = &dataFine;
		param[5].buffer_length = sizeof(MYSQL_TIME);

		break;

	default:
		fprintf(stderr, "\nInvalid condition at %s:%d\n", __FILE__, __LINE__);
		abort();
	}

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "\nCould not bind parameters for mod_piscina\n", true);
	}

	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "\nAn error occurred while getting mod_piscina.");
	}

	printf("\n\nPiscina modificata correttamente!\n\n");
	fflush(stdout);

	mysql_stmt_close(prepared_stmt);

}



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
		printf("\n**** Benvenuto! Cosa posso fare per te? ****\n");

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
			add_impiego(conn);
			break;
		case 2:
			add_insegnante(conn);
			break;
		case 3:
			add_insegnante_corso(conn);
			break;
		case 4:
			add_piscina(conn);
			break;
		case 5:
			add_qualifica(conn);
			break;
		case 6:
			del_insegnante_corso(conn);
			break;
		case 7:
			fine_impiego(conn);
			break;
		case 8:
			get_insegnante(conn);
			break;
		case 9:
			get_impieghi_passati(conn);
			break;
		case 10:
			get_insegnante_piscina(conn);
			break;
		case 11:
			get_piscina(conn);
			break;
		case 12:
			mod_piscina(conn);
			break;
		case 13:
			return;
		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
		}
	}

}