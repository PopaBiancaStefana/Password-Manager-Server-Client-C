#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <stdint.h>
#include "filereaderwriter.h"

/* portul folosit */
#define PORT 2908

/* codul de eroare returnat de anumite apeluri */
extern int errno;

typedef struct thData
{
  int idThread; // id-ul thread-ului tinut in evidenta de acest program
  int cl;       // descriptorul intors de accept
} thData;

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void raspunde(void *);
void readFromClient(int, int);
char *passD(struct person *, int );

struct person *people;

int main()
{
  struct sockaddr_in server; // structura folosita de server
  struct sockaddr_in from;
  int nr; // mesajul primit de trimis la client
  int sd; // descriptorul de socket
  int pid;
  pthread_t th[100]; // Identificatorii thread-urilor care se vor crea
  int i = 0;

  /* crearea unui socket */
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("[server]Eroare la socket().\n");
    return errno;
  }
  /* utilizarea optiunii SO_REUSEADDR */
  int on = 1;
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  /* pregatirea structurilor de date */
  bzero(&server, sizeof(server));
  bzero(&from, sizeof(from));

  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
  server.sin_family = AF_INET;
  /* acceptam orice adresa */
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  /* utilizam un port utilizator */
  server.sin_port = htons(PORT);

  /* atasam socketul */
  if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
  {
    perror("[server]Error at bind().\n");
    return errno;
  }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen(sd, 2) == -1)
  {
    perror("[server]Error at listen().\n");
    return errno;
  }

  // preluam datele din fisier
  people = readFromFile();

  /* servim in mod concurent clientii...folosind thread-uri */
  while (1)
  {
    int client;
    thData *td; // parametru functia executata de thread
    int length = sizeof(from);

    printf("[server]Waiting at port %d...\n", PORT);
    fflush(stdout);

    // client= malloc(sizeof(int));
    /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
    if ((client = accept(sd, (struct sockaddr *)&from, &length)) < 0)
    {
      perror("[server]Error at accept().\n");
      continue;
    }

    /* s-a realizat conexiunea, se astepta mesajul */

    // int idThread; //id-ul threadului
    // int cl; //descriptorul intors de accept

    td = (struct thData *)malloc(sizeof(struct thData));
    td->idThread = i++;
    td->cl = client;

    pthread_create(&th[i], NULL, &treat, td);

  } // while
};
static void *treat(void *arg)
{
  struct thData tdL;
  tdL = *((struct thData *)arg);
  printf("[thread]- %d -  Connected...\n", tdL.idThread);
  fflush(stdout);
  pthread_detach(pthread_self()); // eliberez memoria dupa ce threadul se opreste
  raspunde((struct thData *)arg);
  /* am terminat cu acest client, inchidem conexiunea */
  close((intptr_t)arg);
  return (NULL);
};

void raspunde(void *arg)
{
  int i = 0;
  char client_msg[800] = "";
  char server_msg[800] = "";
  int id_person = 0;

  struct thData tdL;
  struct peron *thisPerson;
  tdL = *((struct thData *)arg);

  char welcome[800] = "\n\n█▀█ ▄▀█ █▀ █▀ █░█░█ █▀█ █▀█ █▀▄   █▀▄▀█ ▄▀█ █▄░█ ▄▀█ █▀▀ █▀▀ █▀█\n█▀▀ █▀█ ▄█ ▄█ ▀▄▀▄▀ █▄█ █▀▄ █▄▀   █░▀░█ █▀█ █░▀█ █▀█ █▄█ ██▄ █▀▄\n\nWelcome!Type a number for a specific command\n\n1.register\n2.login\n3.logout\n4.Add password\n5.Edit password\n6.View all passwords\n7.Delete password\n8.New category\n9.View all categories\n10.Change master password\n\n";

  while (1)
  {
    strcat(server_msg, welcome);

    if (write(tdL.cl, &server_msg, sizeof(char[800])) <= 0)
    {
      printf("[Thread %d] ", tdL.idThread);
      perror("[Thread]Error at write() to client.\n");
    }
    else
      printf("[Thread %d]Message sent.\n", tdL.idThread);

    strcpy(client_msg, "");
    if (read(tdL.cl, &client_msg, sizeof(char[800])) < 0)
    {
      printf("[Thread %d]\n", tdL.idThread);
      perror("Error at read() from client.\n");
    }
    printf("[Thread %d]Message from client: %s\n", tdL.idThread, client_msg);

    server_msg[0] = '\0';

    switch (atoi(client_msg))
    {
    case 1 ... 2:
      printf("[Thread %d]Register or login command: %s\n", tdL.idThread, client_msg);
      char command[800] = "";
      strcpy(command, client_msg);
      char username[800] = "";
      char password[800] = "";
      int done = 0;

      while (done == 0)
      {
        if (username[0] == '\0')
          strcpy(server_msg, "Enter username:\n");
        else if (password[0] == '\0')
          strcpy(server_msg, "Enter master password:\n");

        if (write(tdL.cl, &server_msg, sizeof(char[800])) <= 0)
        {
          printf("[Thread %d] ", tdL.idThread);
          perror("[Thread]Error at write() to client.\n");
        }
        else
          printf("[Thread %d]Message sent.\n", tdL.idThread);

        strcpy(client_msg, "");
        if (read(tdL.cl, &client_msg, sizeof(char[800])) < 0)
        {
          printf("[Thread %d]\n", tdL.idThread);
          perror("Error at read() from client.\n");
        }
        printf("[Thread %d]Message from client: %s\n", tdL.idThread, client_msg);

        if (username[0] == '\0')
          strcpy(username, client_msg);
        else if (password[0] == '\0')
        {
          strcpy(password, client_msg);
          if (strcmp(command, "1") == 0)
          {

            // register
            printf("\nINAINTE %d\n", getNumberOfPeople(people));
            *people = registerPerson(people, username, password);

            strcpy(server_msg, "Registered and connected\n");
            id_person = people[getNumberOfPeople(people) - 1].id;

            printf("\nDUPA %d  : id %d\n", getNumberOfPeople(people), id_person);
          }
          else
          {

            // login
            int check = loginPerson(people, username, password);
            if (check == 0)
            {
              strcpy(server_msg, "User does not exist\n");
            }
            else if (check == -1)
            {
              strcpy(server_msg, "Wrong password\n");
            }
            else
            {
              strcpy(server_msg, "Connected\n");
              id_person = check;
            }
          }
          done = 1;
        }
      }
      break;

    case 3:
      printf("[Thread %d]Logout command: %s\n", tdL.idThread, client_msg);
      id_person = 0;
      strcpy(server_msg, "Logged out\n");
      break;

    case 4:
      // add password
      break;

    case 5:
      // edit password command
      printf("[Thread %d]Edit password command.\n", tdL.idThread);
      char title[800] = "";
      char field[800] = "";
      done = 0;

      if (id_person == 0)
      {
        strcpy(server_msg, "Not logged in.\n");
      }

      while (done == 0 && id_person != 0)
      {
        if (title[0] == '\0')
          strcpy(server_msg, "Enter the title of the password you want to edit:\n ");
        else if (field[0] == '\0')
          strcpy(server_msg, "Enter the field you want to edit (1 for category, 2 for title, 3 for username, 4 for password, 5 for url, 6 for notes):\n");
        else
          strcpy(server_msg, "Enter the new value:\n");

        if (write(tdL.cl, &server_msg, sizeof(char[800])) <= 0)
        {
          printf("[Thread %d] ", tdL.idThread);
          perror("[Thread]Error at write() to client.\n");
        }
        else
          printf("[Thread %d]Message sent.\n", tdL.idThread);

        strcpy(client_msg, "");
        if (read(tdL.cl, &client_msg, sizeof(char[800])) < 0)
        {
          printf("[Thread %d]\n", tdL.idThread);
          perror("Error at read() from client.\n");
        }
        printf("[Thread %d]Message from client: %s\n", tdL.idThread, client_msg);

        if (title[0] == '\0')
          strcpy(title, client_msg);
        else if (field[0] == '\0')
          strcpy(field, client_msg);
        else
        {
          // printf("INAINTE: %s\n", viewPassword(people, id_person, title));
          people = editPassword(people, id_person, title, atoi(field), client_msg);

          for (int i = 0; i < getNumberOfPeople(people); i++)
          {
            printf("id: %d, name: %s, masterPassword: %s, games: %s\n", people[i].id, people[i].name, people[i].masterPassword, people[i].passwords[0].title);
          }

          strcpy(server_msg, "Password updated.\n");
          done = 1;
        }
      }
      break;

    case 6:
      // view all passwords
      printf("[Thread %d]View all passwords command.\n", tdL.idThread);

      if (id_person == 0)
        strcpy(server_msg, "Not logged in.\n");
      else
      {
        char *info = passD(people, 1);
        printf("info: %s\n", info);
      }

      break;

    case 7:
      // delete password
      break;

    case 8:
      // new caregory
      break;

    case 9:
      // view all categories
      break;

    case 10:
      // change master password
      break;
    }
  }
}

void readFromClient(int fd, int idThread)
{
  char client_msg[800] = "";

  if (read(fd, &client_msg, sizeof(char[800])) < 0)
  {
    printf("[Thread %d]\n", idThread);
    perror("Error at read() from client.\n");
  }

  printf("::%s", client_msg);
}

