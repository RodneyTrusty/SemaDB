# REQUIREMENT
Implement a simple file-based database with a command-line interface for the Linux
platform. The database will consist of unique IDs (8 byte unsigned integers). The
command-line interface will allow three commands ‘insert’, ‘find’, ‘delete’.

Usage:

`dbcli <db filename> insert <id>`

inserts the number `<id>` into the database in `<db filename>`

`dbcli <db filename> find <id>`

Search for `<id>` in the database in `<db filename>`. If found, return success exit code. If not
found return failure exit code and print a meaningful error message to stderr.

`dbcli <db filename> delete <id>`

Remove the id number `<id>` from the database `<db filename>`. If a number is removed, a
subsequent ‘find’ should fail.

## Tasks
- Clone this git repository
- Implement main.c and db.c according to the requirements above.
- Implement the functions defined in db.h in db.c
- Implement the command line interface in main.c according to the requirements
  above
- Write unit tests for your db.c implementation in `test_db.c` using https://github.com/ThrowTheSwitch/Unity.
- Write a Makefile (or CMakeLists.txt) that can be used to build the executable dbcli from main.c and db.c, and run the unit tests
- Write a WRITEUP.md markdown file with a description of your submission

## Submission
Push your completed code to your personal github or bitbucket account and notify
us of the URL

### Deliverables
- `main.c` command line interface implementation
- `db.c`  database implementation
- `test_db.c` database implementation
- `Makefile` or `CMakeLists.txt` GNU makefile or CMake file to build the implementation
- `WRITEUP.md` description of your submission

