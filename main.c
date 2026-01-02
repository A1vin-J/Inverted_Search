#include "invsch.h"

int main(int argc, char *argv[])
{
    int f_create = 0, f_update = 0, f_saved = 0;
    ip_list *inputs = NULL;

    // Hash table declaration
    w_node* hash_table[MAX_HASH_SIZE];

    // Initialize hash table
    for (int i = 0; i < MAX_HASH_SIZE; i++)
        hash_table[i] = NULL;

    // Validate CLA inputs
    if(!validate_create(argc, argv, &inputs)) return 0;
    printf("\nInput list :\n");
    print_ip_list(inputs);

    int option;
    
    while(1)
    {
        printf("Menu\n1. Create Database\n2. Display Database\n3. Search Database\n4. Update Database\n5. Save Database\n6. Exit\nEnter option :");
        if(scanf("%d",&option) != 1) option = 0;
        clear_stdin();

        switch(option)
        {
            //Create Database
            case 1:

            //Does Database already exist ?
            if(!f_create && !f_update)
            {
                if(create_db(inputs, hash_table))
                {
                    printf("\nDatabase created successfully!\n\n");
                    f_create = 1;
                }
                else
                {
                    printf("\nDatabase creation failed!\n\n");
                    return 0;
                }
            }
            else
                printf("\nDatabase already exists !\n\n");
            break;

            // Display Database
            case 2:

            // Does Database exist?
            if(f_create || f_update)
            {
                display_db(hash_table);
            }
            else
                printf("\nNo Database is created yet!\n\n");
            break;

            // Search
            case 3:
            {
                // Does Database exist?
                if(f_create || f_update)
                {
                    char s_word[MAX_WORD_SIZE];
                    printf("\nEnter the word to search: ");
                    scanf("%s",s_word);
                    clear_stdin();
                    search(hash_table, s_word);
                }
                else
                    printf("\nNo Database is created yet!\n\n");
                break;
            }

            // Update
            case 4:
            {
                //Does Database already exist ?
                if(!f_create && !f_update)
                {
                    char backup_fname[MAX_FNAME_SIZE];
                    printf("\nEnter the Backup file name : ");
                    scanf("%s",backup_fname);
                    clear_stdin();

                    if(update_db(hash_table, backup_fname))
                    {
                        printf("\nDatabase Updated Succesfully from backup file \"%s\"!\n\n",backup_fname);
                        f_update = 1;

                        // Remove files already included in Database
                        remove_duplicate_inputs(&inputs, hash_table);

                        if(inputs)
                        {
                            //Updated input list
                            printf("Updated input list :\n");
                            print_ip_list(inputs);
                            if(create_db(inputs, hash_table))
                            {
                                printf("New input files are included in Database successfully!\n\n");
                            }
                        }
                        else    printf("\nNo new files to include in database.\n");
                        

                    }
                    else
                        printf("\nUpdating Database Failed!\n\n");
                }
                else
                printf("\nDatabase already created/updated !\n\n");
                break;   
            }

            // Save
            case 5:
            {
                // Does Database exist?
                if(f_create || f_update)
                {
                    char backup_fname[MAX_FNAME_SIZE];
                    printf("\nEnter the Backup file name : ");
                    scanf("%s",backup_fname);
                    clear_stdin();

                    if(save_db(hash_table, backup_fname))
                    {
                        printf("\nDatabase Saved Succesfully in backup file \"%s\"!\n\n",backup_fname);
                        f_saved = 1;
                    }
                    else
                        printf("\nSaving Database Failed!\n\n");
                }
                else
                    printf("\nNo Database exists !\n\n");
                break;
            }

            // Exit
            case 6:
            {
                char op;
                if(!f_saved && (f_create || f_update))
                {
                    printf("\nDatabase is not yet saved\nExit without saving? [y/n]: ");
                    scanf("%c",&op);
                    clear_stdin();
                    if(op == 'n'|| op == 'N')
                    {
                        printf("\n");
                        break;
                    }
                    else if(op == 'y' || op == 'Y') return 0;
                    else
                    {
                        printf("Invalid option!\n\n");
                        break;
                    }
                }
                return 0;
            }

            default:
            printf("\nInvalid option!\nTry again\n\n");
        }
    }
    return 0;
}