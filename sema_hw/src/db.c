#include <stdio.h> 
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include "../inc/db.h"




//Command #'s 0, 1, 2 respectively
char * dbcli_commands[3] = {"insert","find","delete"};



db_t * db_new(){

    //Allocate mem for struct
    db_t *dbs = malloc(sizeof(db_t)); 

    //Allocate mem for file read
    dbs->data = (char**)(malloc(MAX_DB * sizeof(char*)));

    return dbs;

};

//Open DB file and load to data struct
int db_open(db_t * db, const char * filename){

    db->delimiter = strdup(",");
    char * token;                        

    db->db_file = fopen(filename, db->open_mode);

    if(db->db_file == NULL){ 

        printf("File was not opened. Errno = %d\n",errno);

        return errno;

    }

    //Get data from file, store in db->data
    while(fgets(db->packet, PAC_SIZE, db->db_file) != NULL)
    {

        int tok_num = 0;

        db->data[tok_num] = strtok(db->packet, db->delimiter);
        
     	while (db->data[tok_num] != NULL){
            
            tok_num++;

            db->data[tok_num] = strtok(NULL, db->delimiter);
            
        }

        db->token_cnt = tok_num;
    }

     return 0;
}



int db_insert(db_t *db, uint64_t id){


        long disk_space = get_free_space(DB_DIR);

        db->open_mode = "r"; 

        //Open file and store contents in db->data
        if(db_open(db, db->filename) != 0)
        {
            return errno;
        }; 

        //Check if token value less than max allowed by this program and
        //disk space available. Max size configurable by editing MAX_DB macro
        if(db->token_cnt < MAX_DB | ((db->token_cnt*2) < disk_space)){
            
            //0: ID does NOT exist, 1: ID already xists
            uint8_t id_exists = 0;

            char * num = malloc(ceil((log10(id) + 1) * sizeof(char))); 

            sprintf(num, "%lu", id);

            for(uint64_t x = 0; x < db->token_cnt; x++){

                if(strcmp(db->data[x], num) == 0){

                    id_exists = 1;
                    break;

                };

            }   

            if(id_exists){

                printf("Index Already Exists\n");

                errno = EEXIST;
                return errno;

            }

            //Append user number to data retrieved from db
            db->data[db->token_cnt] = num;

            //Create temporary file
            FILE * tmp = fopen("tmp.txt", "w");

            //Write contents to temporary file  
            //No comma prepending first entry
            fprintf(tmp,"%s",db->data[0]); 

            //Write the rest of the tokens
            for(int x = 1; x <= db->token_cnt; x++){

                fprintf(tmp,",%s",db->data[x]);

            }

            free(num);

            fclose(db->db_file);
            fclose(tmp);

            remove(db->filename);   

            //Rename temporary file
            //TODO: Make temporaty file name some derivation of 
            //original file name to prevent potential conflicts
            rename("tmp.txt", db->filename);
            
            //Notify User of Success
            printf("Insert Successful\n");
            return 0;
        }

        else{

            printf("Not enough Memory. Insert Aborted\n");

            errno = ENOMEM;
            return errno;

        }
    
};


int db_find(db_t *db, uint64_t id){


    //If file does NOT exist
    if(access(db->filename, F_OK) != 0)
    { 

    printf("Database does NOT exist\n");

    errno = ENOENT;

    return errno;

    }

    //0: Match Not Found, 1: Match Found
    uint8_t match_found = 0;

    char * num = malloc(ceil((log10(id) + 1) * sizeof(char))); 

    sprintf(num, "%lu", id);

    db->open_mode = "r"; 

    //Open file and store contents in db->data
    if(db_open(db, db->filename) != 0){

        return errno;

    }; 

    //Cycle through data searching for match
    for(uint64_t x = 0; x < db->token_cnt; x++)
    {

        if(strcmp(db->data[x], num) == 0)
        {

            match_found = 1;

            break;

        };

    }   

    free(num);

    if(match_found){

        printf("Match Found!\n");
        
        return 1;
    }

    else{

        printf("Match NOT Found!\n");

        return 0;
    }

}


int db_delete(db_t *db, uint64_t id){

    //If file does NOT exist
    if(access(db->filename, F_OK) != 0){ 

    printf("Database does NOT exist\n");

    errno = ENOENT;

    return errno;

    }

    //0: ID does NOT exist, 1: ID already xists
    uint8_t id_exists = 0;

    char * num = malloc(ceil((log10(id) + 1) * sizeof(char))); 

    sprintf(num, "%lu", id);

    db->open_mode = "r"; 

    //Open file and store contents in db->data
    if(db_open(db, db->filename) != 0){

        return errno;

    }; 

    //Check if id present in file
    for(uint64_t x = 0; x < db->token_cnt; x++){

        if(strcmp(db->data[x], num) == 0){

            id_exists = 1;
            break;

        };
    }  


    if(id_exists){

        FILE * tmp = fopen("tmp.txt", "w");

        //Write contents to temporary file  
        //No comma prepending first entry
        fprintf(tmp,"%s",db->data[0]); 

        //Write the rest of the tokens
        for(int x = 1; x < db->token_cnt; x++){

           //Skip the ID that we want to delete
           if(strcmp(db->data[x], num) != 0){

            fprintf(tmp,",%s",db->data[x]);

           }

        }

        free(num);

        fclose(db->db_file);
        fclose(tmp);

        remove(db->filename);   

        rename("tmp.txt", db->filename);
            
        printf("%lu successfully deleted from database\n", id);

        return 1;

    }

    else{

        printf("%lu does not exist in specified database \n", id);

        return 0;

    }

};


void db_free(db_t * db){

    free(db->data);

    free(db);

}

int check_char(char * s, char * c){ 
  
    for(int x = 0; x < strlen(s); x++)
    {

        for(int i = 0; i < strlen(c); i++)
        {
            
            if(s[x] == c[i])
            {
                return -1;
            }
            
        }

    }

    return 0;

} 


int db_sanitize(char * command_token[])
{
    //Implement basic error checking, could be improved
    //Rules:
    //No Token should be empty
    //Trim all Tokens
    //First token should match client name
    //Second token should have a "." and we will limit it to 50 characters
    //No illegal chars allowed in filename
    //Third token should be one of the pre-defined commands
    //Fourth token should be converted to a 64bit unsigned int


    char illegal_chars[] = {'<','>',':','\"','/','|','?','*'};


    //0: No command token is NULL, 1: At least 1 command token is NULL
    uint8_t empty_check = 0;

    //If any part of command is NULL set empty_check to 1
    for(uint8_t x = 0; x < 4; x++){
   
        if(command_token[x] == NULL | command_token[x] == ""){

        empty_check = 1;

        break;

    }

}

    if(empty_check == 1){

        printf("Command Not Recognized\n");
        
        return -1;

    }


    //Trim all tokens
    for(uint8_t x = 0; x < 4; x++){

        trim(command_token[x]);

    }

    //Check if first element matches client name
    if(strcmp(command_token[0], DBCLI) != 0){

        printf("%s is not recognized. Did you mean dbcli?\n", command_token[0]);
        
        return -2; 
    }

    //File must be less than 50 chars
    if(strlen(command_token[1]) > 50){

        printf("File name too long.\n");
        
        return -3;

    }
    



    for(int i = 0; i < strlen(illegal_chars); i++)
    {
        //0: No illegal char found 1: Illegal char found
        uint8_t clean = 0;

        if(strchr(command_token[1], illegal_chars[i]) != NULL)
        {
            clean = 1;
        }

        for(int x = 0; x < strlen(command_token[1]); x++)
        {

            if(command_token[1][x] < 33 | command_token[1][x] > 126)
            {

                clean = 1;

            }

        }

        if(clean == 1)
        {

            printf("Illegal characters found in file name. Please use a different file name.\n");

            return -6; 

        }
        
    }  

    //0: User command is invalid, 1: User command is valid
    uint8_t command_match = 0; 
    int command_num = 0;

    //Search for a match in pre-defined commands
    for(uint8_t x = 0; x < 3; x++){

        if(strcmp(command_token[2], dbcli_commands[x]) == 0){ // Match found
    
            command_match = 1; 
            command_num = x;

        }

    }   

    if(command_match == 0){

        printf("Command %s is not a valid command.\n", command_token[2]);
    
        return -4; 
    
    }

    char *pntr;
    uint64_t index = 0;

    //Reject ID if number not entered
    if(!(index = strtol(command_token[3], &pntr, 10))){
    
        printf("You entered an invalid ID\n");
        return -5;
    
    }

    if(index == 0x7FFFFFFFFFFFFFFF)
    {
        
        printf("Index too large! Drop it down.\n");

        return -7;

    }

    return command_num;
}

int trim(char * str) {
	int trimmed = 0;
	int n;
	int len;
 
	len = strlen(str);

	n = len - 1;

	//Trim right space
	while((n>=0) && isspace(str[n])) {

		str[n] = '\0'; 
		trimmed += 1;
		n--;

	}
 
	//Trim left space
	n = 0;
	while((n < len) && (isspace(str[0]))) {
		str[0] = '\0'; 
		str = str + 1;
		trimmed += 1;
		n++;
	}

	return trimmed;
}

long get_free_space(const char* path)
{

  struct statvfs stat;
  
  if (statvfs(path, &stat) != 0) {
    // error
    return -1;
  }

  return stat.f_bsize * stat.f_bavail;
}