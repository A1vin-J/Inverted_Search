#include "invsch.h"

int get_hash_index(char *word)
{
    if (isalpha(word[0]))
        return tolower(word[0]) - 'a';   // 0–25 buckets for alphabets
    else
        return 26;                       // special bucket for numbers and punctuations
}

w_node *search_word(w_node *hash_table[], char *word)
{
    // Get the index of the bucket
    int index = get_hash_index(word);

    // Temp pointer for traversing the list
    w_node *temp = hash_table[index];

    while (temp)
    {
        // If foud returnn the address of the word node
        if (strcmp(temp->word, word) == 0)
            return temp;
        temp = temp->next_word;
    }
    return NULL;
}

int insert_word(w_node *hash_table[], char *word, char *fname)
{
    // Get the index of the bucket
    int index = get_hash_index(word);

    // Searching if the word already exists
    w_node *wn = search_word(hash_table, word);

    // Word already exists
    if (wn)
    {
        // Temp pointer for file node traversal
        f_node *fn = wn->info;

        // File already exist
        while (fn)
        {
            if (strcmp(fn->fname, fname) == 0)
            {
                fn->count++;
                return 1;
            }
            fn = fn->next_file;
        }

        // New file entry
        f_node *newf = malloc(sizeof(f_node));
        if (!newf)
            return 0;

        strcpy(newf->fname, fname);
        newf->count = 1;
        newf->next_file = wn->info;
        wn->info = newf;
        wn->file_count++;

        return 1;
    }
    // New word entry
    else
    {
        w_node *neww = malloc(sizeof(w_node));
        f_node *newf = malloc(sizeof(f_node));

        if (!neww || !newf)
        {
            free(neww);
            free(newf);
            return 0;
        }

        // Inserting word 
        strcpy(neww->word, word);
        neww->next_word = hash_table[index];
        hash_table[index] = neww;
        neww->file_count = 1;

        // Inserting file name
        strcpy(newf->fname, fname);

        // Initializing count and next_file pointer
        newf->count = 1;
        newf->next_file = NULL;

        neww->info = newf;

        return 1;
    }
}