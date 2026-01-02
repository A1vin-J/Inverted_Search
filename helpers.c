#include "invsch.h"

int validate_create(int argc, char *argv[], ip_list **inputs)
{
    if (argc < 2)
    {
        printf("Usage: ./a.out <file1.txt> <file2.txt> ...\nAtleast 1 \"%s\" file is required\n", ".txt");
        return 0;
    }

    for (int i = 1; i < argc; i++)
    {
        // If the file name is valid
        if (is_valid(*inputs, argv[i]))
        {
            ip_list *new = malloc(sizeof(ip_list));
            if (!new)
            {
                printf("Memory allocation failed\n");
                continue;
            }

            strcpy(new->fname, argv[i]);
            new->next_fname = NULL;

            if (*inputs == NULL)
            {
                *inputs = new;
            }
            else
            {
                ip_list *temp = *inputs;
                while (temp->next_fname)
                    temp = temp->next_fname;
                temp->next_fname = new;
            }

            printf("\n-> %s added to input list\n", argv[i]);
        }
    }

    // No valid file names are found
    if(*inputs == NULL)
    {
        printf("\nNo valid input files !\n");
        return 0;
    }

    return 1;
}

int is_valid(ip_list *head, char *fname)
{
    FILE *fp;

    // is .txt file ?
    char *dot = strrchr(fname, '.');
    if (!dot || strcmp(dot, ".txt") != 0)
    {
        printf("\nInvalid: %s is not a .txt file\n", fname);
        return 0;
    }

    // is file present ?
    fp = fopen(fname, "r");
    if (!fp)
    {
        printf("\nInvalid: %s file not present\n", fname);
        return 0;
    }

    // is file empty ?
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0)
    {
        printf("\nInvalid: %s file is empty\n", fname);
        fclose(fp);
        return 0;
    }
    fclose(fp);

    // is already in list ?
    while (head)
    {
        if (strcmp(head->fname, fname) == 0)
        {
            printf("\nInvalid: %s already exists in list\n", fname);
            return 0;
        }
        head = head->next_fname;
    }

    // Valid
    return 1;
}

void clear_stdin(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void remove_newline(char *str)
{
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

int is_file_in_db(w_node *hash_table[], const char *fname)
{
    for (int i = 0; i < MAX_HASH_SIZE; i++)
    {
        w_node *wtemp = hash_table[i];
        while (wtemp)
        {
            f_node *ftemp = wtemp->info;
            while (ftemp)
            {
                if (strcmp(ftemp->fname, fname) == 0)
                    return 1;
                ftemp = ftemp->next_file;
            }
            wtemp = wtemp->next_word;
        }
    }
    return 0;
}

void remove_duplicate_inputs(ip_list **inputs, w_node *hash_table[])
{
    ip_list *cur = *inputs;
    ip_list *prev = NULL;

    while (cur)
    {
        if (is_file_in_db(hash_table, cur->fname))
        {
            printf("Skipping %s (already included)\n", cur->fname);

            ip_list *tmp = cur;

            if (prev == NULL)
            {
                *inputs = cur->next_fname;
                cur = *inputs;
            }
            else
            {
                prev->next_fname = cur->next_fname;
                cur = prev->next_fname;
            }

            free(tmp);
        }
        else
        {
            prev = cur;
            cur = cur->next_fname;
        }
    }
}

void print_ip_list(ip_list *inputs)
{
    ip_list *head = inputs;

    while(head)
    {
        printf("%s -> ", head->fname);
        head = head->next_fname;
    }
    printf("NULL\n\n");
}