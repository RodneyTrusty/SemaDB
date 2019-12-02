#include "../inc/db.h"
#include "../unity/src/unity.h"

//db_open() defines
#define FILE_OPENED              (0)

//Filename Indexes
#define FN_NORMAL                (0)
#define FN_NO_EXIST              (1)

char * filenames[] = 
{

    //Normal Filename, File Exists
    DB_DIR"test.txt",

    //File that doesn't exist. Delete if accidentally created
    DB_DIR"delete_me.txt"

};


void setUp(void)
{

}

void tearDown(void)
{

}


void test_db_insert(void)
{

    db_t * db = db_new(); 

    //File exists index exists
    strncpy(db->filename,filenames[FN_NORMAL], DB_MAX_NAME_SIZE);
    TEST_ASSERT_EQUAL_INT(EEXIST,db_insert(db, 1));

    //File exists index doesn't
    strncpy(db->filename,filenames[FN_NORMAL], DB_MAX_NAME_SIZE);
    TEST_ASSERT_EQUAL_INT(0,db_insert(db, 10));
    db_delete(db, 10);

    db_free(db);

}

void test_db_find(void)
{

    db_t * db = db_new(); 

    //File not found
    strncpy(db->filename,filenames[FN_NO_EXIST], DB_MAX_NAME_SIZE);
    TEST_ASSERT_EQUAL_INT(ENOENT,db_find(db, 1));

    //File found Index not found
    strncpy(db->filename,filenames[FN_NORMAL], DB_MAX_NAME_SIZE);
    TEST_ASSERT_EQUAL_INT(0 ,db_find(db, 98769));

    //File found index found
    strncpy(db->filename,filenames[FN_NORMAL], DB_MAX_NAME_SIZE);
    TEST_ASSERT_EQUAL_INT(1 ,db_find(db, 1));

    db_free(db);


}

void test_db_delete(void)
{

    db_t * db = db_new(); 


    //File does NOT exist
    strncpy(db->filename,filenames[FN_NO_EXIST], DB_MAX_NAME_SIZE);
    TEST_ASSERT_EQUAL_INT(ENOENT, db_delete(db, 45));

    //File Exists index exists

    db_insert(db, 10);

    strncpy(db->filename,filenames[FN_NORMAL], DB_MAX_NAME_SIZE);
    TEST_ASSERT_EQUAL_INT(0, db_delete(db, 10));

    //File Exists index does NOT exist
    strncpy(db->filename,filenames[FN_NO_EXIST], DB_MAX_NAME_SIZE);
    TEST_ASSERT_EQUAL_INT(ENOENT, db_delete(db, 10));

    db_free(db);

}

void test_db_open(void)
{


    db_t * db = db_new(); 
    db->open_mode = "r";

    //File doesn't exist shouldn't open
    strncpy(db->filename,filenames[FN_NO_EXIST], DB_MAX_NAME_SIZE);
    TEST_ASSERT_EQUAL_INT(errno,db_open(db, db->filename));

    //File exists open it
    strncpy(db->filename,filenames[FN_NORMAL], DB_MAX_NAME_SIZE);
    TEST_ASSERT_EQUAL_INT(FILE_OPENED,db_open(db, db->filename));

    db->open_mode = "w+"; 

    //File doesn't exist, mode set to w+ file should be created
    strncpy(db->filename,filenames[FN_NO_EXIST], DB_MAX_NAME_SIZE);
    TEST_ASSERT_EQUAL_INT(FILE_OPENED,db_open(db, db->filename));

    remove(db->filename);   

    //db_open() should have loaded file data
    //Compare file data to expected data
    char cmp[2];
    for(uint8_t x = 0 ; x < 10; x++)
    {
        //Print x to strinf
        sprintf(cmp, "%d", x);

        //Fail test if data doesn't match
        if(strcmp(db->data[x], cmp) != 0)
        {
            TEST_FAIL();
        }

    }

}


void test_db_sanitize()
{

    char * tokens[4];

    tokens[0] = malloc(sizeof(char)*10);
    tokens[1] = malloc(sizeof(char)*60);
    tokens[2] = malloc(sizeof(char)*10);
    tokens[3] = malloc(sizeof(char)*20);

    //Stadard entry, should throw no errors
    strcpy(tokens[0], "dbcli");
    strcpy(tokens[1], "test");
    strcpy(tokens[2], "insert");
    strcpy(tokens[3], "9999999");
    TEST_ASSERT_GREATER_THAN(-1, db_sanitize(tokens));


    //Index out of bounds
    strcpy(tokens[3],"9223372036854775807"); //Maximum uint64_t value
    TEST_ASSERT_EQUAL_INT(-7,db_sanitize(tokens));


    //CLient not recognized
    strcpy(tokens[0],"wrongCli");
    strcpy(tokens[2],"insert");
    strcpy(tokens[3],"9999999");
    TEST_ASSERT_EQUAL_INT(-2, db_sanitize(tokens));

    strcpy(tokens[0],"");
    strcpy(tokens[2],"insert");
    strcpy(tokens[3],"9999999");
    TEST_ASSERT_EQUAL_INT(-2, db_sanitize(tokens));

    //Filename too long
    strcpy(tokens[0],"dbcli");
    strcpy(tokens[1],"filename_too_long_filename_too_long_filename_too_long_");
    strcpy(tokens[2],"insert");
    strcpy(tokens[3],"9999999");
    TEST_ASSERT_EQUAL_INT(-3, db_sanitize(tokens));

    strcpy(tokens[0],"dbcli");
    strcpy(tokens[1],"");
    strcpy(tokens[2],"insert");
    strcpy(tokens[3],"9999999");
    TEST_ASSERT_EQUAL_INT(-0, db_sanitize(tokens));




    const char * illegal_chars[] = {"<test",">test",":test","\"test","/test","|test","?test","*test"};

    for(int x = 0; x < (sizeof(illegal_chars)/sizeof(illegal_chars[0])); x++){
        
        strcpy(tokens[1], illegal_chars[x]);

        TEST_ASSERT_EQUAL_INT(-6, db_sanitize(tokens));

    }

    for(int x = 0; x < 4; x++)
    {

        free(tokens[x]);

    }

    //Invalid command
    tokens[0] = "dbcli";
    tokens[1] = "test";
    tokens[2] = "usurp";
    tokens[3] = "9999999";
    TEST_ASSERT_EQUAL_INT(-4, db_sanitize(tokens));

    tokens[0] = "dbcli";
    tokens[1] = "test";
    tokens[2] = "";
    tokens[3] = "9999999";
    TEST_ASSERT_EQUAL_INT(-4, db_sanitize(tokens));

    //Invalid ID
    tokens[0] = "dbcli";
    tokens[1] = "test";
    tokens[2] = "insert";
    tokens[3] = "index_number";
    TEST_ASSERT_EQUAL_INT(-5, db_sanitize(tokens));

    tokens[0] = "dbcli";
    tokens[1] = "test";
    tokens[2] = "insert";
    tokens[3] = "";
    TEST_ASSERT_EQUAL_INT(-5, db_sanitize(tokens));



}


int main(void){

   UNITY_BEGIN();
   RUN_TEST(test_db_find);
   RUN_TEST(test_db_delete);
   RUN_TEST(test_db_open);
   RUN_TEST(test_db_sanitize);
   RUN_TEST(test_db_insert);
   return UNITY_END();

}