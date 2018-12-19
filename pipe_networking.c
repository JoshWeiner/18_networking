#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  mkfifo("PP", 0600);
  	int pp = open("PP", O_RDONLY | O_CREAT, 0600);
	if(pp == -1){
	  printf("Error opening Public Pipe: %s\n", strerror(errno));
	  exit(0);
	} 
  else {
	  printf("PP opened\n");
	}
	char * client_stream = malloc(100);
  	read(pp, client_stream, sizeof(client_stream));
  	printf("Message recieved from pp: %s\n", client_stream);
	remove("pp");
	*to_client = open(client_stream, O_WRONLY);
	if(*to_client == -1){
	  printf("Error opening %s: %s\n", client_stream, strerror(errno));
	  exit(0);
	} 
  else {
	  printf("Opened cts\n");
	}
	write(*to_client, "hello from the Weiner", sizeof(message));
	printf("Sending message through %s:\n", client_stream);
	free(client_stream);
  	return pp;
}


/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  return 0;
}
