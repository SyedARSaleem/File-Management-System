
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

const char* ReturnFileType()
{
    int acc = 0;
    printf("\n-----------------FILE TYPE OPTIONS---------------------");
    printf("\n1 - Text file");
    printf("\n2 - Doc file");
    printf("\n3 - Binary file");
    printf("\nEnter the one of the above options: ");
    scanf("%d",&acc);

    switch(acc)
    {
        case 1:
            return ".txt";
        case 2:
            return ".doc";
        case 3:
            return ".bin";
        default:
            printf("\nThe option doesn't exist, try entering options 1 to 2");
            return ReturnFileType();
    }
}

const char* ReturnFileName(char filname[], char* type)
{
    char *name = filname;
   
    while (*name)
    {
        name++;        
    }
   
    while (*type)
    {
        *name = *type;
        name++;
        type++;
    }
    *name = '\0';
   
    return filname;
}

bool file_exists(const char* fname)
{
    struct stat buffer;
    return stat(fname, &buffer) == 0 ? true : false ;
}

void createfile()
{
    //taking file name and type
    char name[50];  
    printf("\nEnter the name of the file: ");
    scanf("%s", name);
    
    const char* type = ReturnFileType();
    const char* filename = ReturnFileName(name, type);
    //checking if file exists
    if (file_exists(filename))
    {
        printf("\nFile %s already exists, please try again with a different file name", filename);
        createfile();
    }
    else
    {
        FILE *fileptr ;
        fileptr = fopen(filename,"w");
        fclose(fileptr);
        printf("\nFile %s accepted and created",filename);
    }
}
    

void FilePermission(const char *fname)
{
    int perm;  
    char cmd[32];
    int ret = 0; 
    printf("\n-----------------FILE PERMISSION OPTIONS---------------------");
    printf("\n1 - Read Only");
    printf("\n2 - Read and Write");
    printf("\n3 - Read, Write And Execute");
    printf("\n4 - Write Only");
    printf("\nEnter the one of the above options: ");
    scanf("%d",&perm);

    switch (perm)
    {
        case 1:
            sprintf(cmd, "chmod 444 %s", fname);
            break;
        case 2:
            sprintf(cmd, "chmod 666 %s", fname);
            break;
        case 3:
            sprintf(cmd, "chmod 777 %s", fname);
            break;
        case 4:
            sprintf(cmd, "chmod 222 %s", fname);
            break;
        default:
            printf("/nSorry the option is out of range, please enter number from 1 to 4: ");
            FilePermission(fname);
    }
    ret = system(cmd);
    if (ret == 0)
        printf("\nPermission of the file changed succesfully");
    else
        printf("\nUnable to change the permission of the file");
}

void change_permission()
{
    char name[20];
    printf("\nEnter the file name to change the permission: ");
    scanf("%s",name);
    const char* type = ReturnFileType();
    const char* filename = ReturnFileName(name, type);
    if (file_exists(filename))
        FilePermission(filename);
    else
    {
        printf("\nSorry the filename you entered doesnt exist, Try again");
        change_permission();
    }
}

void encryptdecrypt(const char *fname, int EorD)
{
    // 1 for encryption
    FILE *fptr = fopen(fname, "r");
    if (fptr == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Open a temporary file for writing
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL)
    {
        perror("Error creating temporary file");
        fclose(fptr);
        exit(EXIT_FAILURE);
    }

    char ch;
    int shift = EorD ? 3 : -3;

    // Read character by character
    while ((ch = fgetc(fptr)) != EOF)
    {
        if ((ch >= 'A' && ch <= 'Z'))
        {
            // Process uppercase letters
            ch = (ch - 'A' + shift + 26) % 26 + 'A';
        }
        else if ((ch >= 'a' && ch <= 'z'))
        {
            // Process lowercase letters
            ch = (ch - 'a' + shift + 26) % 26 + 'a';
        }
        else if (ch >= '0' && ch <= '9')
        {
            // Process digits
            ch = (ch - '0' + shift + 10) % 10 + '0';
        }
        else if (ch >= '!' && ch <= '/')
        {
            // Process some special characters
            ch = (ch - '!' + shift + 15) % 15 + '!';
        }
        else if (ch >= ':' && ch <= '@')
        {
            // Process some special characters
            ch = (ch - ':' + shift + 7) % 7 + ':';
        }

        // Print or write the encrypted/decrypted character
        printf("%c", ch);
        fputc(ch, tempFile);
    }

    fclose(fptr);
    fclose(tempFile);

    // Replace the original file with the temporary file
    remove(fname);
    rename("temp.txt", fname);
}

void check_encryption(int ED)
{
    char name[20];
    printf("\nEnter the name of file: ");
    scanf("%s",name);
    const char* type = ReturnFileType();
    const char* filename = ReturnFileName(name, type);
    if (file_exists(filename))
        encryptdecrypt(filename, ED);
    else
    {
        printf("\nSorry the filename you entered doesnt exist, Try again");
        check_encryption(ED);
    }
}

void deletefile()
{
    char name[20];
    printf("\nEnter the name of file: ");
    scanf("%s",name);
    const char* type = ReturnFileType();
    const char* filename = ReturnFileName(name, type);
    if (file_exists(filename))
    {
        remove(filename);
        printf("%s deleted successfully",filename);
    }
    else
    {
        printf("\nSorry the filename you entered doesnt exist, Try again");
        deletefile();
    }
}

int main()
{
    int check;
    char path[128];
    char dname[20];
    printf("Enter the name of the directory: ");
    scanf("%s",dname);
    // creating directory
    char* dirname = dname;

    check = mkdir(dirname,0777);

    if (!check)
        printf("Directory created!\n");
    else
    {
        printf("Unable to create directory, Try again\n");
        EXIT_FAILURE;
    }

// changing focus of directory
    if (chdir(dirname) < 0)
    {
        printf("Change of directory Unsuccesful \n");
        EXIT_FAILURE;
    }

    int choice = 1;
    while ((choice > 0)&&(choice < 6))
    {
        printf("\n-----------------FUNCTION CHOICES---------------------");
        printf("\n1 - Create a file");
        printf("\n2 - Delete a file");
        printf("\n3 - Change permission for a file");
        printf("\n4 - Encrypt a file");
        printf("\n5 - Decrypt a file");
        printf("\n6 - Finish the program");
        printf("\n\nEnter the number for the function you want to perform: ");
        scanf("%d",&choice);

        switch (choice)
        {
            case 1:
                createfile();
                break;
            case 2:
                deletefile();
                break;
            case 3:
                change_permission();
                break;
            case 4:
                check_encryption(1);
                break;
            case 5:
                check_encryption(0);
                break;
            case 6:
                printf("Exiting program, Thank you!");
                exit(0);

            default:
                printf("/nSorry the option is out of range, please enter number from 1 to 6: ");
        }
    }

    return 0;
 
}
