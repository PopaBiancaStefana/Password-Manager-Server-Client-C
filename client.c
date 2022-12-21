
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

int main(int argc, char *argv[])
{
  int sd;                    // descriptorul de socket
  struct sockaddr_in server; // structura folosita pentru conectare
                             // mesajul trimis
  char server_msg[4000] = "";
  char client_msg[4000] = "";

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
  {
    printf("Sintax %s <adresa_server> <port>\n", argv[0]);
    return -1;
  }

  /* stabilim portul */
  port = atoi(argv[2]);

  /* cream socketul */
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("Error at socket().\n");
    return errno;
  }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons(port);

  /* ne conectam la server */
  if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
  {
    perror("[client]Error at connect().\n");
    return errno;
  }


  while(1){
    /* citirea raspunsului dat de server (apel blocant pina cind serverul raspunde) */
    if (read(sd, &server_msg, sizeof(char[4000])) < 0)
    {
      perror("[client]Error at read() from server.\n");
      return errno;
    }
    /* afisam mesajul primit */
    printf("[client]Message from server:\n\n%s\n", server_msg);

    if(strcmp(server_msg, "Exit") == 0)
      break;

    /* citirea mesajului */
    printf("[client]Command: ");
    strcpy(client_msg, "");
    fflush(stdout);
    fgets(client_msg, sizeof(client_msg), stdin);
    // read(0, client_msg, sizeof(client_msg));
    client_msg[strlen(client_msg) - 1] = '\0';
    

    printf("[client]We got [%s]\n", client_msg);

    /* trimiterea mesajului la server */
    if (write(sd, &client_msg, sizeof(char[4000])) <= 0)
    {
      perror("[client]Error at write() to server.\n");
      return errno;
    }

    
  }
 

  /* inchidem conexiunea, am terminat */
  close(sd);
}