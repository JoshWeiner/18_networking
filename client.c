#include "pipe_networking.h"

int main() {
  int to_server, from_server;

  char buff[BUFFER_SIZE];

  from_server = client_handshake( &to_server );

}
