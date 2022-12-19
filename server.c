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

  struct thData tdL;
  tdL = *((struct thData *)arg);

  char welcome[800] = "\n\n█▀█ ▄▀█ █▀ █▀ █░█░█ █▀█ █▀█ █▀▄   █▀▄▀█ ▄▀█ █▄░█ ▄▀█ █▀▀ █▀▀ █▀█\n█▀▀ █▀█ ▄█ ▄█ ▀▄▀▄▀ █▄█ █▀▄ █▄▀   █░▀░█ █▀█ █░▀█ █▀█ █▄█ ██▄ █▀▄\n\nWelcome!Type a number for a specific command\n\n1.register\n2.login\n3.logout\n4.Add password\n5.Edit password\n6.View password\n7.Delete password\n8.New category\n9.View category\n10.Change master password\n\n";

  while (1)
  {
    /*pregatim mesajul de raspuns */

    // printf("[Thread %d]Message from server: %s\n", tdL.idThread, welcome);
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

    // edit password command
    //  if(strcmp(client_msg, "5") == 0){
    //    strcpy(server_msg, "Enter the name of the password you want to edit: ");
    //    if (write(tdL.cl, &server_msg, sizeof(char[800])) <= 0)
    //    {
    //      printf("[Thread %d] ", tdL.idThread);
    //      perror("[Thread]Error at write() to client.\n");
    //    }
    //    else
    //      printf("[Thread %d]Message sent.\n", tdL.idThread);

    //   strcpy(client_msg, "");
    //   if (read(tdL.cl, &client_msg, sizeof(char[800])) < 0)
    //   {
    //     printf("[Thread %d]\n", tdL.idThread);
    //     perror("Error at read() from client.\n");
    //   }
    // }
    server_msg[0] = '\0';
    char *info = viewPassword(people, 1, "csgo");
    strcpy(server_msg, info);
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