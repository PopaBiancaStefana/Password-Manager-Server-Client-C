#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct password
{
    char category[30];
    char title[30];
    char username[30];
    char passwrd[30];
    char url[30];
    char notes[50];
};

typedef struct person
{
    int id;
    char name[20];
    char masterPassword[20];
    char categories[20][30];
    struct password passwords[20];
    
};

void addPerson(struct person person){

    FILE *outfile;

    // open file for writing
    outfile = fopen("person.dat", "a");
    if (outfile == NULL)
    {
        fprintf(stderr, "\nError opened file\n");
        exit(1);
    }

    // write struct to file
    
    fwrite(&person, sizeof(struct person), 1, outfile);

    if (fwrite == 0)
        printf("error writing file !\n");
    

    fclose(outfile);
}

/*
void addPeopleToFile(struct person input[])
{

    FILE *outfile;

    // open file for writing
    outfile = fopen("person.dat", "a");
    if (outfile == NULL)
    {
        fprintf(stderr, "\nError opened file\n");
        exit(1);
    }

    printf(sizeof(input));

    // write struct to file
    for (int i = 0; i < sizeof(input); i++)
    {
        fwrite(&input[i], sizeof(struct person), 1, outfile);

        if (fwrite == 0)
            printf("error writing file !\n");
    }

    fclose(outfile);
}*/

struct person* readFromFile()
{

    FILE *infile;
    struct person input;
    struct person people[100];

    infile = fopen("person.dat", "r");
    if (infile == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit(1);
    }

    int count = 0;
    while (fread(&input, sizeof(struct person), 1, infile))
    {
        //printf("id = %d name = %s %s %s %s\n", input.id,
              // input.name, input.categories[1], input.passwords[0].title, input.passwords[1].title);
        people[count] = input;
        count++;
    }

    fclose(infile);
    struct person *returningPeople = people;

    return returningPeople;
}

void editPassword(int id, char *title, char *field, char *new_value)
{
}

void deletePassword(int id, char *title)
{
  
}

void viewPassword(int id, char *title)
{
}

void addCategory(int id, char *new_category)
{
}

void viewCategory(int id, char *new_category)
{
}

void changeMasterPassword(int id)
{
}

void changeName(int id)
{
}

void findPerson(char *name)
{
}

int main()
{
    struct person input1 = {1, "alin", "69dsdfs", {"games", "school"}, {{"games", "csgo", "alintdg", "parola", "", "e veche"}, {"school", "docs", "alintutz", "parola22", "", "ok"}}};
    struct person input2 = {2, "elena", "6sdsafs", {"games", "acasa"}, {{"games", "lol", "nebula", "alorap", "haide.com", ""}, {"games", "dbdl", "balenciaga", "parlica", "", "brnr"}}};
   
    //addPerson(input1);
    //addPerson(input2);
  

    // deletePassword(2, "lol");

    struct person *people = readFromFile();

    printf("%s %s %s %s\n", people[0].name, people[1].categories[0],people[1].categories[1], people[0].passwords[1].passwrd);

    return 0;
}
