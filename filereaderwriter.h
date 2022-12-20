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
        // input.name, input.categories[1], input.passwords[0].title, input.passwords[1].title);
        people[count] = input;
        count++;
    }

    fclose(infile);
    struct person *returningPeople = people;

    return returningPeople;
}

int getNumberOfPeople(struct person *people)
{
    int count = 0;
    for (int i = 0; i < sizeof(people); i++)
    {
        if (people[i].id != 0)
            count++;
        else
            break;
    }
    return count;
}

struct person *editPassword(struct person *people, int id, char *title, int field, char *new_value)
{
    for (int i = 0; i < getNumberOfPeople(people); i++)
    {

        if (people[i].id == id)
        {
            for (int j = 0; j < 20; j++)
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

char *viewAllPasswords(struct person *people, int id)
{
    char *info;
    char *temp;
    strcpy(info, "");

    for (int i = 0; i < getNumberOfPeople(people); i++)
    {

        if (people[i].id == id)
        {
            for (int j = 0; j < 20; j++)
            {
                if (strcmp(people[i].passwords[j].title, "") != 0)
                {
                    sprintf(temp, "\ncategory: %s,\ntitle: %s,\nusername: %s, \npassword: %s, \nurl: %s, \nnotes: %s\n", people[i].passwords[j].category, people[i].passwords[j].title, people[i].passwords[j].username, people[i].passwords[j].passwrd, people[i].passwords[j].url, people[i].passwords[j].notes);
                    strcat(info, temp);
                }
            }
            break;
        }
    }

    info[strlen(info) - 1] = '\0';
    return info;
}

struct person *addCategory(int id, char *new_category)
{
}

// struct person *addPassword(int id, )

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

int findPersonByName(struct person *people, char *name)
{
    for (int i = 0; i < sizeof(people); i++)
    {
        if (strcmp(people[i].name, name) == 0)
        {
            return people[i].id;
        }
    }
    return 0;
}

struct person *findPersonById(struct person *people, int id)
{
    for (int i = 0; i < sizeof(people); i++)
    {
        if (people[i].id == id)
        {
            return &people[i];
        }
    }
    return NULL;
}

int loginPerson(struct person *people, char *name, char *password)
{
    int id = findPersonByName(people, name);

    if (id != 0)
    {
        if (strcmp(people[id - 1].masterPassword, password) == 0)
        {
            return id; // correct password and name
        }
    }
    else
        return 0; // wrong name

    return -1; // wrong password
}

struct person registerPerson(struct person *people, char *name, char *password)
{
    int id = findPersonByName(people, name);
    if (id == 0)
    {
        char categories[30][30];
        char passwords[30][30];
        int position = getNumberOfPeople(people);
        people[position].id = position + 1;
        strcpy(people[position].name, name);
        strcpy(people[position].masterPassword, password);
    }

    return *people;
}
