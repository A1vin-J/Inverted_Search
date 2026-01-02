#include "invsch.h"

// Create Database
int create_db(ip_list *inputs, w_node *hash_table[])
{
    char raw[MAX_WORD_SIZE];
    char token[MAX_WORD_SIZE];

    while (inputs)
    {
        FILE *fp = fopen(inputs->fname, "r");
        if (!fp)
        {
            inputs = inputs->next_fname;
            continue;
        }

        while (fscanf(fp, "%49s", raw) == 1)
        {
            // Copy raw token
            strcpy(token, raw);

            // Strip ONLY trailing punctuations
            int len = strlen(token);
            while (len > 0 && ispunct(token[len - 1]))
            {
                token[len - 1] = '\0';
                len--;
            }

            // If token became empty -> check for standalone punctuations
            if (token[0] == '\0')
            {
                if (!isalpha(raw[0]))   // Standalone punctuations / special
                    strcpy(token, raw);
                else
                    continue;
            }

            // Insert into hash table (bucket decided inside hash.c)
            insert_word(hash_table, token, inputs->fname);
        }

        fclose(fp);
        inputs = inputs->next_fname;
    }

    return 1;
}


// Display Database
void display_db(w_node *hash_table[])
{
    printf("\n ======================= INVERTED SEARCH DATABASE ========================\n");

    printf(" -------------------------------------------------------------------------\n");
    printf("|%-6s | %-15s | %-10s | %-19s | %-10s |\n"," Index","     Word","File Count","     File Name","Word Count");
    printf(" -------------------------------------------------------------------------\n");

    for (int i = 0; i < MAX_HASH_SIZE; i++)
    {
        if (hash_table[i] == NULL)
            continue;

        w_node *wtemp = hash_table[i];

        // Word nodes traversal
        while (wtemp)
        {
            f_node *ftemp = wtemp->info;
            int first = 1;

            // File nodes traversal
            while (ftemp)
            {
                if (first)
                {
                    printf("|   %-3d | %-15s |      %-3d   | %-19s |      %-5d |\n",i,wtemp->word,wtemp->file_count,ftemp->fname,ftemp->count);
                    first = 0;
                }
                else
                {
                    printf("|%6s | %-15s | %-10s | %-19s |      %-5d |\n",""," ","",ftemp->fname,ftemp->count);
                }
                ftemp = ftemp->next_file;
            }

            printf(" -------------------------------------------------------------------------\n");
            wtemp = wtemp->next_word;
        }
    }
    printf("\n");
}


// Search Word
void search(w_node *hash_table[], char *word)
{
    char token[MAX_WORD_SIZE];

    // Copy raw token
    strcpy(token, word);

    // Strip ONLY trailing punctuations
    int len = strlen(token);
    while (len > 0 && ispunct(token[len - 1]))
    {
        token[len - 1] = '\0';
        len--;
    }

    // If token became empty -> check for standalone punctuations
    if (token[0] == '\0')
    {
        if (!isalpha(word[0]))   // Standalone punctuations (or) special
            strcpy(token, word);
    }

    w_node *wtemp = search_word(hash_table, token);

    if(wtemp)
    {
        printf("\nWord \"%s\" found in %d file(s)\n", word, wtemp->file_count);

        f_node *ftemp = wtemp->info;

        while (ftemp)
        {
            printf("-> %-10s : %d times\n",ftemp->fname,ftemp->count);
            ftemp = ftemp->next_file;
        }

        printf("\n");
    }
    else
        printf("\nWord not found in Database!\n\n");
}


// Update Database
int update_db(w_node *hash_table[], char *fname)
{
    // File existence check
    FILE *fp = fopen(fname, "r");
    if (!fp)
    {
        printf("\nFile \"%s\" does not exist!\n", fname);
        return 0;
    }

    char line[MAX_UPDATE_LINE_SIZE];

    // File empty check
    if (!fgets(line, sizeof(line), fp))
    {
        printf("\n%s is empty!\n",fname);
        fclose(fp);
        return 0;
    }

    remove_newline(line);

    // Valid Database file check
    if (strcmp(line, "##") != 0)
    {
        printf("\n%s is not a database file !\n",fname);
        fclose(fp);
        return 0;
    }

    int index = -1;

    while (fgets(line, sizeof(line), fp))
    {
        remove_newline(line);

        // Index - line
        if (strncmp(line, "#:", 2) == 0)
        {
            sscanf(line, "#:%d", &index);
            continue;
        }

        // Word - line
        if (index >= 0)
        {
            char *tok = strtok(line, ":");
            if (!tok)
                continue;

            // Word
            w_node *wn = malloc(sizeof(w_node));
            strcpy(wn->word, tok);
            wn->next_word = hash_table[index];
            hash_table[index] = wn;

            // File count
            tok = strtok(NULL, ":");
            wn->file_count = atoi(tok);

            wn->info = NULL;

            // File nodes
            for (int i = 0; i < wn->file_count; i++)
            {
                char *fname_tok = strtok(NULL, ":");
                char *count_tok = strtok(NULL, ":");

                if (!fname_tok || !count_tok)
                    break;

                f_node *fn = malloc(sizeof(f_node));
                strcpy(fn->fname, fname_tok);
                fn->count = atoi(count_tok);

                fn->next_file = wn->info;
                wn->info = fn;
            }
        }
    }

    fclose(fp);
    return 1;
}


// Save Database
int save_db(w_node *hash_table[], char *fname)
{
    char *dot = strrchr(fname, '.');
    if (!dot || strcmp(dot, ".txt") != 0)
    {
        printf("Invalid: %s is not a .txt file\n", fname);
        return 0;
    }

    FILE *fp = fopen(fname, "w");
    if(!fp)
    {
        printf("Error opening %s file", fname);
        return 0;
    }
    
    fprintf(fp, "%s\n", "##");

    int first;

    for (int i = 0; i < MAX_HASH_SIZE; i++)
    {
        if (hash_table[i] == NULL)
            continue;

        fprintf(fp, "#:%d\n",i);
        w_node *wtemp = hash_table[i];

        while (wtemp)
        {
            f_node *ftemp = wtemp->info;
            first = 1;

            while (ftemp)
            {
                if (first)
                {
                    fprintf(fp, "%s:%d:%s:%d:",wtemp->word,wtemp->file_count,ftemp->fname,ftemp->count);
                    first = 0;
                }
                else
                {
                    fprintf(fp, "%s:%d:",ftemp->fname,ftemp->count);
                }
                ftemp = ftemp->next_file;
            }
            fprintf(fp, "#\n");
            wtemp = wtemp->next_word;
        }
    }

    fclose(fp);
    return 1;
}