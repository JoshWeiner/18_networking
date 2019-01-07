#include "pipe_networking.h"


int server_setup() {
  printf("[server] establishing wkp\n");
  mkfifo("server", 0600);
  int from_client = open("server", O_RDONLY);
  remove("server");
  printf("[server] parent server initialized\n");
  return from_client;
}


int server_connect(int from_client) {
  char msg[HANDSHAKE_BUFFER_SIZE];

  read(from_client, msg, sizeof(msg));
  int to_client = open(msg, O_WRONLY);

  write(to_client, msg, sizeof(msg));

  read(from_client, msg, sizeof(msg));
  printf("[server] handshake message from client: %s\n", msg);

  return to_client;
}

/*=========================
  server_handshake
  args: int * to_client
  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake (int *to_client) {

  int from_client = server_setup();
  char msg[HANDSHAKE_BUFFER_SIZE];

  read(from_client, msg, sizeof(msg));

  //connect to client, send message
  *to_client = open(msg, O_WRONLY);
  write(*to_client, msg, sizeof(msg));

  //read for client
  read(from_client, msg, sizeof(msg));
  printf("[server] handshake received: %s\n", msg);

  return from_client;
}

/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int from_server;
  char msg[HANDSHAKE_BUFFER_SIZE];

  printf("[client]: handshake: connecting to wkp\n");
  *to_server = open( "server", O_WRONLY, 0);
  if ( *to_server == -1 )
    exit(1);
  sprintf(msg, "%d", getpid() );
  mkfifo(msg, 0600);

  write(*to_server, msg, sizeof(msg));
  from_server = open(msg, O_RDONLY, 0);
  read(from_server, msg, sizeof(msg));
  printf("[server]: pipe id [%s]\n", msg);

  remove(msg);
  printf("[client] handshake: removed private pipe\n");
  write(*to_server, ACK, sizeof(msg));

  return from_server;
}
