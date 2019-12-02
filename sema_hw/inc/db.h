
#ifndef DB_H
#define DB_H

#include <stdint.h> // uint64_t
#include <errno.h> // EEXIST, ENOMEM, etc
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>

#define MAX_DB             1000      //Maxumim allowed DB indexes. Could be much larger
#define PAC_SIZE           200       //Packet size for fgets. Could be much larger.
#define DBCLI             "dbcli"
#define DB_DIR            "db/"
#define DB_MAX_NAME_SIZE   100       //more than enough




//Command #'s 0, 1, 2 respectively
char * dbcli_commands[3];







//DB object
typedef struct db {
    FILE *db_file;
    char filename[DB_MAX_NAME_SIZE];
    int token_cnt;
    char * delimiter;
    char * open_mode;
    char ** data;
    char packet[PAC_SIZE];
}db_t;

/**
 * Database constructor 
 *
 * @return dynamically allocated and initialized database object
*/ 
db_t * db_new();

/**
 * Open/create a database using the provided filename
 *
 * @param db the database object
 * @param filename the file to open/create
 *
 * @return 0 on success, -errno for other errors
 */
int db_open(db_t * db, const char * filename);


/**
 * Insert the id into the database
 *
 * @param db the database object
 * @param id the id number to insert
 *
 * @return 0 on success, -errno on error.
 *         Specifically: 
 *         -EEXIST if the id is already in the database
 *         -ENOMEM if storage space is exhausted.
 */
int db_insert(db_t *db, uint64_t id);

/**
 * Search for an id in the database
 *
 * @param db the database object
 * @param id the id number to find
 *
 * @return: 1 if found, 0 if not found, -errno on error
 */
int db_find(db_t *db, uint64_t id);

/**
 * Delete the id from the database
 *
 * @param db the database object
 * @param id the id number to delete
 *
 * @return 1 if deleted, 0 if not found, or -errno on error
 */
int db_delete(db_t *db, uint64_t id);

/**
 * Release any dynamically allocated resources
 * opened or created by the db functions.
 *
 * @param db the database object to cleanup 
 *
 */
void db_free(db_t *db);

/**
 * Validate user db call input
 * 
 * @param  command_token char array of database call elements
 *
 * @return -1 if 1 or more elements NULL, -2 if client name not specified, -3 if file name too long
 *         -4 if invalid command, -5 if index not a number, -6 if illegal chars found in file name
 *         -7 if index number too large, 1..3 if db call accepted return requested command number
 * 
 */
int db_sanitize(char * command_token[]);


/**
 * Utility function to trim whitespace on either side of string
 *
 * @param str string to trim
 *
 * @return Number of whitespace chars trimmed
 * 
 */
int trim(char * str);

/**
 * Utility function to return free space on disk
 *
 * @param path path to file you want to store
 *
 * @return bytes free
 * 
 */
long get_free_space(const char* path);

#endif /* __DBREC_H__ */
