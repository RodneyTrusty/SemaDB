#include <stdio.h> 
#include <stdint.h>
#include <stdlib.h>
#include "../inc/db.h"
#include <unistd.h>

#define CMD_MAX 100

//Command number definitions
#define DB_INSERT  0
#define DB_FIND    1
#define DB_DELETE  2



int main() { 
 
    //Raw user input
    char shell_input[CMD_MAX];

    //User input tokens
    char * command_token[4];

    uint8_t tok_num = 0; 

while(1){
    

    for(uint8_t x = 0; x < 4; x++){

        command_token[x] = NULL;

    }

    printf("Please Enter A Command: ");

    fgets(shell_input, CMD_MAX, stdin); 

    command_token[tok_num] = strtok(shell_input, " ");
   
    while( command_token[tok_num] != NULL) {

        tok_num++;
        
        command_token[tok_num] = strtok(NULL, " ");

    }

    tok_num = 0; 


    //Sanitize user input and copy into variables
    int command_num = db_sanitize(command_token);

    if(command_num < 0)
    {
        continue;
    }

    char *ptr;
    uint64_t index = strtol(command_token[3], &ptr, 10);

    db_t * db = db_new(); 

    char fn[] = DB_DIR;

    strcat(fn, command_token[1]);

    strcat(fn, ".txt");

    strncpy(db->filename, fn, DB_MAX_NAME_SIZE);


    if(command_num == DB_INSERT)
    {
        //Check if file exists
        if(access(db->filename, F_OK) == 0)
        {

            db_insert(db, index);

        }

        else
        {

            char create[3];

            //0: Do not open file and insert. 1: Open file and insert 
            uint8_t fopen = 0; 

            //Loop until Yes/No decision made 
            while(create[0] != 'Y' && create[0] != 'y' && create[0] != 'N' && create[0] != 'n')
            {

                printf("Database doesn't exist. Would you like to create it? Y/N: ");

                fgets(create, 0x03, stdin); 

                if(create[0] == 'Y' | create[0] == 'y')
                {
        
                    //Create and open file
                    db->open_mode = "w+"; 

                    db_open(db, db->filename); 

                    fprintf(db->db_file,"%lu",index);

                    fclose(db->db_file);

                    printf("New Database Created with ID:%li\n", index);

                }

                else if(create[0] == 'N' | create[0] == 'n')
                {

                    printf("New Database NOT Created\n");
                    break;
                }

                else
                {
            
                printf("Invalid Option\n");

                }

            }

    
        }

    }

    else if(command_num == DB_FIND)
    {
        db_find(db, index);
    }

    else if(command_num == DB_DELETE)
    {
        db_delete(db, index);
    }


    db_free(db);


}

    return 0; 

}