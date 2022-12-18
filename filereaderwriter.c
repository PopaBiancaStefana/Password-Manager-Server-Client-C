#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>  

struct password
{
    char category[30];
    char title[30];
    char username[30];
    char passwrd[30];
    char url[30];
    char notes[50];
    bool deleted;
};

typedef struct person
{
    int id;
    char name[20];
    char masterPassword[20];
    char categories[20][30];
    struct password passwords[20];
    bool deleted;
};

// void addPerson(struct person );
// void readFromFile();
// void editPassword(int, char , char, char );

void addPerson(struct person input)
{

    FILE *outfile;

    // open file for writing
    outfile = fopen("person.dat", "a");
    if (outfile == NULL)
    {
        fprintf(stderr, "\nError opened file\n");
        exit(1);
    }

    // write struct to file
    fwrite(&input, sizeof(struct person), 1, outfile);

    if (fwrite != 0)
        printf("contents to file written successfully !\n");
    else
        printf("error writing file !\n");

    fclose(outfile);
}

void readFromFile()
{

    FILE *infile;
    struct person input;

    // Open person.dat for reading
    infile = fopen("person.dat", "r");
    if (infile == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit(1);
    }

    // read file contents till end of file
    while (fread(&input, sizeof(struct person), 1, infile))
        printf("id = %d name = %s %s %s %s\n", input.id,
               input.name, input.categories[1], input.passwords[0].title, input.passwords[1].title);

    // close file
    fclose(infile);
}

void editPassword(int id, char* title, char* field, char* new_value)
{
}

void deletePassword(int id, char* title)
{
   FILE *infile;
    struct person input;

    // Open person.dat for reading
    infile = fopen("person.dat", "ra");
    if (infile == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit(1);
    }

    // read file contents till end of file
    while (fread(&input, sizeof(struct person), 1, infile))
        printf("id = %d name = %s %s %s %s\n", input.id,
               input.name, input.categories[1], input.passwords[0].title, input.passwords[1].title);

    // close file
    fclose(infile);   
}

void viewPassword(int id, char* title)
{
}

void addCategory(int id, char* new_category)
{
}

void viewCategory(int id, char* new_category)
{
}

void changeMasterPassword(int id)
{
}

void changeName(int id)
{
}

void findPerson(char* name)
{
}



int main()
{
    struct person input1 = {1, "alin", "69dsdfs", {"games", "school"}, {{"games", "csgo", "alintdg", "parola", "", "e veche"}, {"school", "docs", "alintutz", "parola22", "", "ok"}}};
    struct person input2 = {3, "elena", "6sdsafs", {"games", "acasa"}, {{"games", "lol", "nebula", "alorap", "haide.com", ""}, {"games", "dbdl", "balenciaga", "parlica", "", "brnr"}}};

    //addPerson(input1);
    //addPerson(input2);

    //

    deletePassword(2, "lol");

    //readFromFile();

    return 0;
}
