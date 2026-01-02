#ifndef INV_SCH
#define INV_SCH

// Header files
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Macros
#define MAX_HASH_SIZE 27
#define MAX_FNAME_SIZE 50
#define MAX_WORD_SIZE 50
#define MAX_UPDATE_LINE_SIZE 512

// Input file names list
typedef struct ip_list
{
    char fname[MAX_FNAME_SIZE];
    struct ip_list *next_fname;
} ip_list;


// File node
typedef struct f_node
{
    char fname[MAX_FNAME_SIZE];
    int count;
    struct f_node *next_file;
}f_node;


// Word node
typedef struct w_node
{
    int file_count;
    char word[MAX_WORD_SIZE];
    f_node *info;
    struct w_node *next_word;
}w_node;


    /* Functionalities [operations.c]*/

// Creating Database
int create_db(ip_list *inputs, w_node *hash_table[]);

// Displaying the Database
void display_db(w_node *hash_table[]);

// Search a word in the hash table
void search(w_node *hash_table[], char *word);

// Save Database
int save_db(w_node *hash_table[], char *fname);

// Update Database from backup file
int update_db(w_node *hash_table[], char *fname);


    /* Hashing related functions [hash.c] */

// Getting index of bucket array
int get_hash_index(char *word);

// Searching for a word
w_node *search_word(w_node *hash_table[], char *word);

// Insert a word in the hash tabe
int insert_word(w_node *hash_table[], char *word, char *fname);


    /* Helper Functions [ helpers.c] */

// Validate and create input list
int validate_create(int argc, char *argv[], ip_list **inputs);

// Check input is valid or not
int is_valid(ip_list *head, char *fname);

// Clear the input buffer
void clear_stdin(void);

// Remove the newline 
void remove_newline(char *str);

// Check file is already included in Database
int is_file_in_db(w_node *hash_table[], const char *fname);

// Remove files already included in Database
void remove_duplicate_inputs(ip_list **inputs, w_node *hash_table[]);

// Printing input list
void print_ip_list(ip_list *inputs);

#endif