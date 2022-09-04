#pragma once

#include <stdbool.h>
#include <mysql.h>

struct configuration {
	char* host;
	char* db_username;
	char* db_password;
	unsigned int port;
	char* database;

	char username[128];
	char password[128];
};

extern struct configuration conf;
extern char* pool;

extern int parse_config(char* path, struct configuration* conf);
extern bool setup_prepared_stmt(MYSQL_STMT** stmt, char* statement, MYSQL* conn);
extern void print_stmt_error(MYSQL_STMT* stmt, char* message);
extern void set_binding_param(MYSQL_BIND* param, enum enum_field_types type, void* buffer, unsigned long len);
extern char multiChoice(char* domanda, char choices[], int num);
extern bool yesOrNo(char* domanda, char yes, char no, bool predef, bool insensitive);