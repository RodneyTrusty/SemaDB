#ifndef DB_H
#define DB_H

#include <stdint.h> // uint64_t
#include <errno.h> // EEXIST, ENOMEM, etc

// Opaque db object
// TODO define 'struct db' in your private implementation
// file
typedef struct db db_t;

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


#endif /* __DBREC_H__ */
