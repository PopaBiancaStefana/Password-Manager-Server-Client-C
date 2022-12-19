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

void addPerson(struct person person)
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

    fwrite(&person, sizeof(struct person), 1, outfile);

    if (fwrite == 0)
        printf("error writing file !\n");

    fclose(outfile);
}

void addPeopleToFile(struct person *input)
{

    FILE *outfile;

    // open file for writing
    outfile = fopen("person.dat", "w");
    if (outfile == NULL)
    {
        fprintf(stderr, "\nError opened file\n");
        exit(1);
    }

    // write struct to file
    for (int i = 0; i < sizeof(input); i++)
    {
        fwrite(&input[i], sizeof(struct person), 1, outfile);

        if (fwrite == 0)
            printf("error writing file !\n");
    }

    fclose(outfile);
}

struct person *readFromFile()
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
        // printf("id = %d name = %s %s %s %s\n", input.id,
        //  input.name, input.categories[1], input.passwords[0].title, input.passwords[1].title);
        people[count] = input;
        count++;
    }

    fclose(infile);
    struct person *returningPeople = people;

    return returningPeople;
}

struct person *editPassword(struct person *people, int id, char *title, int field, char *new_value)
{
    for (int i = 0; i < sizeof(people); i++)
    {

        if (people[i].id == id)
        {
            for (int j = 0; j < sizeof(people[i].passwords); j++)
            {

                if (strcmp(people[i].passwords[j].title, title) == 0)
                {

                    switch (field)
                    {
                    case 1: // category
                        strcpy(people[i].passwords[j].category, new_value);
                        break;

                    case 2: // title
                        strcpy(people[i].passwords[j].title, new_value);
                        break;

                    case 3: // username
                        strcpy(people[i].passwords[j].username, new_value);
                        break;

                    case 4: // passwrd
                        strcpy(people[i].passwords[j].passwrd, new_value);
                        break;

                    case 5: // url
                        strcpy(people[i].passwords[j].url, new_value);
                        break;

                    case 6: // notes
                        strcpy(people[i].passwords[j].notes, new_value);
                        break;

                    default:
                        break;
                    }
                    break;
                }
            }
            break;
        }
    }

    return people;
}

struct person *deletePassword(int id, char *title)
{
}

char *viewPassword(struct person *people, int id, char *title)
{
    char *info;

    for (int i = 0; i < sizeof(people); i++)
    {

        if (people[i].id == id)
        {
            for (int j = 0; j < sizeof(people[i].passwords); j++)
            {

                if (people[i].passwords[j].title != NULL && strcmp(people[i].passwords[j].title, title) == 0)
                {
                    sprintf(info, "category: %s,\ntitle: %s,\nusername: %s, \npassword: %s, \nurl: %s, \nnotes: %s\n", people[i].passwords[j].category, people[i].passwords[j].title, people[i].passwords[j].username, people[i].passwords[j].passwrd, people[i].passwords[j].url, people[i].passwords[j].notes);
                    break;
                }
            }
            break;
        }
    }

    return info;
}

struct person *addCategory(int id, char *new_category)
{
}

void viewCategory(int id, char *new_category)
{
}

struct person *changeMasterPassword(int id)
{
}

struct person *changeName(struct person *people, int id)
{
    strcpy(people[0].name, "AAALIN");
    return people;
}

void findPerson(char *name)
{
}

/*
int main()
{
    struct person input1 = {1, "alin", "69dsdfs", {"games", "school"}, {{"games", "csgo", "alintdg", "parola", "", "e veche"}, {"school", "docs", "alintutz", "parola22", "", "ok"}}};
    struct person input2 = {2, "elena", "6sdsafs", {"games", "acasa"}, {{"games", "lol", "nebula", "alorap", "haide.com", ""}, {"games", "dbdl", "balenciaga", "parlica", "", "brnr"}}};


    // struct person manyP[2] ={{1, "alin", "69dsdfs", {"games", "school"}, {{"games", "csgo", "alintdg", "parola", "", "e veche"}, {"school", "docs", "alintutz", "parola22", "", "ok"}}},{2, "elena", "6sdsafs", {"games", "acasa"}, {{"games", "lol", "nebula", "alorap", "haide.com", ""}, {"games", "dbdl", "balenciaga", "parlica", "", "brnr"}}}};

    // deletePassword(2, "lol");

    struct person *people = readFromFile();

    // people = editPassword(people, 2, "lol", 2, "mario");


    // for (int i = 0; i < sizeof(people); i++)
    // {
    //     if(people[i].id != 0){
    //         printf("\n%s %s %s %s %s\n", people[i].name, people[i].passwords[0].title, people[i].passwords[0].passwrd, people[i].passwords[1].title, people[i].passwords[1].passwrd);
    //     }
    // }

    char* info = viewPassword(people,1,"csgo");
    printf("%s", info);


    //addPeopleToFile(people);
    // printf("%s %s %s\n", people[0].name, people[0].passwords->category ,people[0].passwords[0].passwrd);
    // printf("%s %s %s %s\n", people[0].name, people[1].categories[0], people[1].categories[1], people[0].passwords[1].passwrd);

    return 0;
}
*/