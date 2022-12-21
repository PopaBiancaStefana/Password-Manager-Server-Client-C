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


void addPersonToFile(struct person myPerson)
{

    FILE *infile, *copy;

    // open file for writing
    infile = fopen("person.dat", "r");
    copy = fopen("copy.dat", "a");

    if (infile == NULL)
    {
        fprintf(stderr, "\nError opened file\n");
        exit(1);
    }

   // printf("id= %d, name = %s\n", myPerson->id, myPerson->name);

    struct person temporary;
    while (fread(&temporary, sizeof(struct person), 1, infile))
    {
          printf("din fisier id= %d, name = %s\n", temporary.id,  temporary.name);
        if (temporary.id != myPerson.id)
        {
            fwrite(&temporary, sizeof(struct person), 1, copy);

            if (fwrite == 0)
                printf("error writing file !\n");
        }
    }

    //struct person temp;
    //copy myPerson to temp
    // temp.id = myPerson->id;
    // strcpy(temp.name, myPerson->name);
    // strcpy(temp.masterPassword, myPerson->masterPassword);
    // for (int i = 0; i < 20; i++)
    // {
    //     strcpy(temp.categories[i], myPerson->categories[i]);
    // }
    // for (int i = 0; i < 20; i++)
    // {
    //     strcpy(temp.passwords[i].category, myPerson->passwords[i].category);
    //     strcpy(temp.passwords[i].title, myPerson->passwords[i].title);
    //     strcpy(temp.passwords[i].username, myPerson->passwords[i].username);
    //     strcpy(temp.passwords[i].passwrd, myPerson->passwords[i].passwrd);
    //     strcpy(temp.passwords[i].url, myPerson->passwords[i].url);
    //     strcpy(temp.passwords[i].notes, myPerson->passwords[i].notes);
    // }

    fwrite(&myPerson, sizeof(struct person), 1, copy);

    if (fwrite == 0)
        printf("error writing file !\n");

    fclose(infile);
    fclose(copy);

   remove("person.dat");
   rename("copy.dat", "person.dat");

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

void deletePassword(struct person *myPerson, char *title)
{
    for (int j = 0; j < 20; j++)
    {
        if (strcmp(myPerson->passwords[j].title, title) == 0)
        {
            strcpy(myPerson->passwords[j].category, "");
            strcpy(myPerson->passwords[j].title, "");
            strcpy(myPerson->passwords[j].username, "");
            strcpy(myPerson->passwords[j].passwrd, "");
            strcpy(myPerson->passwords[j].url, "");
            strcpy(myPerson->passwords[j].notes, "");
            break;
        }
    }

    // return myPerson;
}

char *viewPasswords(struct person *myPerson)
{
    char *chunk = malloc(4000);
    char *info = malloc(4000);

    for (int j = 0; j < 20; j++)
    {
        if (myPerson->passwords[j].title[0] != '\0')
        {
            snprintf(chunk, 4000, "\ncategory: %s,\ntitle: %s,\nusername: %s, \npassword: %s, \nurl: %s, \nnotes: %s\n", myPerson->passwords[j].category, myPerson->passwords[j].title, myPerson->passwords[j].username, myPerson->passwords[j].passwrd, myPerson->passwords[j].url, myPerson->passwords[j].notes);
            strcat(info, chunk);
        }
    }
    return info;
}

struct person *addCategory(int id, char *new_category)
{
}

void addPassword(struct person *myPerson, char *category, char *title, char *username, char *passwrd, char *url, char *notes)
{
    for (int i = 0; i < 20; i++)
    {
        if (myPerson->passwords[i].title[0] == '\0')
        {
            strcpy(myPerson->passwords[i].category, category);
            strcpy(myPerson->passwords[i].title, title);
            strcpy(myPerson->passwords[i].username, username);
            strcpy(myPerson->passwords[i].passwrd, passwrd);
            strcpy(myPerson->passwords[i].url, url);
            strcpy(myPerson->passwords[i].notes, notes);
            break;
        }
    }
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

struct person *findPersonById(int id)
{
    struct person *people = readFromFile();

    for (int i = 0; i < sizeof(people); i++)
    {
        if (people[i].id == id)
        {
            return &people[i];
        }
    }
    return NULL;
}

int loginPerson(char *name, char *password)
{
    struct person *people = readFromFile();

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

struct person *registerPerson(char *name, char *password)
{
    struct person *people = readFromFile();

    int id = findPersonByName(people, name);
    if (id == 0)
    {
        struct person *myPerson;
        int position = getNumberOfPeople(people);
        myPerson->id = position + 1;
        strcpy(myPerson->name, name);
        strcpy(myPerson->masterPassword, password);
        return myPerson;
    }
    else
        return NULL;
}

int main()
{
    struct person input1 = {1, "alin", "a", {"games", "school"}, {{"games", "csgo", "alintdg", "parola", "", "e veche"}, {"school", "docs", "alintutz", "parola22", "", "ok"}}};
    struct person input2 = {2, "elena", "6sdsafs", {"games", "acasa"}, {{"games", "lol", "nebula", "alorap", "haide.com", ""}, {"games", "dbdl", "balenciaga", "parlica", "", "brnr"}}};

    //addPerson(input1);
    //addPerson(input2);
    //  struct person manyP[2] ={{1, "alin", "69dsdfs", {"games", "school"}, {{"games", "csgo", "alintdg", "parola", "", "e veche"}, {"school", "docs", "alintutz", "parola22", "", "ok"}}},{2, "elena", "6sdsafs", {"games", "acasa"}, {{"games", "lol", "nebula", "alorap", "haide.com", ""}, {"games", "dbdl", "balenciaga", "parlica", "", "brnr"}}}};

    // deletePassword(2, "lol");

    struct person input3 = {3, "bianca", "ddds", {"dd", "as"}, {{"asd", "asd", "dasd", "sd", "haide.com", ""}, {"games", "dbdl", "baladasenciaga", "dsd", "", "sada"}}};

   

    // printf("count %d", getNumberOfPeople(people));

    // people = editPassword(people, 1, "csgo", 1, "mama");

    //*people = registerPerson(people, "nami", "bastard");

    struct person *thisPerson = &input3;
   addPersonToFile(*thisPerson);
    //print nr of people
    // struct person *people = readFromFile();

    // printf("person %d", findPersonByName(people,"alin"));

    // int num[2] = {1,2};
    // int *ptr = &num;

    // int num2[2] = *(int *)ptr;

    // printf("num2: %d %d\n", num2[0], num2[1]);



    // print people
    //  for (int i = 0; i < getNumberOfPeople(people); i++)
    //  {
    //      printf("id: %d, name: %s, masterPassword: %s, games: %s\n", people[i].id, people[i].name, people[i].masterPassword, people[i].passwords[0].title);
    //  }

    // struct person *thePerson = &input1;

    // addPassword(thePerson, "alo", "das", "nunu", "uku", "fff", "e trrt");

    // deletePassword(thePerson, "docs");

    // char *info = printPers(thePerson);
    // printf("info: %s\n", info);

    // struct person *thisPerson;
    // thisPerson = registerPerson("alin", "69dsdfs");
    // if (thisPerson == NULL)
    // {
    //     printf("User already exists\n");
    // }
    // else
    // {
    //     printf("Registered and connected  %s\n", thisPerson->name);
    // }

    return 0;
}
