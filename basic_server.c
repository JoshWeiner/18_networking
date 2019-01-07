#include "pipe_networking.h"
#include <string.h>
#include <signal.h>

void subserver(int from_client);

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("luigi");
    exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);

  while(1) {
    int from_client = server_setup();
    if(!fork()) {
      subserver(from_client);
      exit(0);
    }
  }
}

void subserver(int from_client) {
  int to_client = server_connect(from_client);
  char msg[BUFFER_SIZE];

      while(read(from_client, msg, BUFFER_SIZE)) {
        printf("[client] %s\n", msg);
        strcat(msg, " -- copy that, client");
        write(to_client, msg, BUFFER_SIZE);
      }

  close(to_client);
  close(from_client);
}
