# SemaDB

SemaDB is a simple file-based database with a command-line interface built for the Linux platform

## Installation

Download and run the sema_db.exe file found in the sema_hw/ directory  

SemaDB can be built using the Makefile found in the sema_hw/ directory. A **test** target is also included for unit testing using the [Throw the swith Unity platform](https://github.com/ThrowTheSwitch/Unity).


## Usage

The SemaDB client follows a very basic syntax with only 3 possible commands: insert, find, and delete.

The syntax is as follows:
```bash
dbcli <db name> <cmd> <index>
```
A few rules apply when making a SemaDB call:  
* Each db call must be prefixed with **dbcli**
* \<db name> must NOT contain illegal characters [< ,> ,: ," ,/ ,| ,? ,* ] and must be less than or equal to 50 characters in length
* \<cmd> must be one of the 3 case sensitive commands allowed by the client [insert, delete, find]
* \<index> must be a unique unsigned 64 bit integer 


### Insert
The **insert** command, as it's name suggests, can be used to insert an index into an existing database. Simply name the database you'd like to access and the index you'd like to insert. By default, the maximum allowed number of indexes in a database is 1000. This is configurable by editing the MAX_DB macro in sema_hw/src/db.h. Databases are saved in the db/ folder.

```bash
## Insert the index "1991" into the "myDB" database

dbcli myDB insert 1991
```
If the index already exists, the insert will be aborted and the following message will be returned:
```bash
Index Already Exists
```
If the database does NOT exist, you will be asked if you want to create it

```bash
Database does NOT exist. Would you like to create it? Y/N: 
```
Selecting Y creates a new database with the specified name  
Selecting N aborts the insert

### Find
The find command searches the specified database and confirms wheter or not a specified index exists or not. 
```bash
## Look for the index "1991" inside the "myDB" database

dbcli myDB find 1991
```
The client follows with either
```bash
Match Found
```
or 
```bash
Match NOT Found
```

### Delete
The delete command searches the specified database and deletes the specified input if it exists
```bash
## Delete the index "1991" from the "myDB" database

dbcli myDB delete 1991
```
A successful deletion would return
```bash
1991 successfully deleted from database
```
If the database does NOT exist the client returns
```bash
Database does NOT exist
```
If the index does not exist in the database the client returns
```bash
1991 does NOT exist inside specified database
```