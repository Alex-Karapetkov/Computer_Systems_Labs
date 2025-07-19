#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "utils.h"

/* Basic framework for a web server than can handle a single request, then
   shuts down. Looks for files in the current directory.

   If the file is found, returns the total length of the response (including
   the HTTP headers). If not, returns -1 after writing a 404 message to the
   socket. Also returns -1 if any errors occur along the way.
 */
ssize_t
serve_web (char *protocol)
{
  // Pass the protocol (should be a port number) to setup_server() to get the
  // server socket.
  int socketfd = setup_server (protocol);
  if (socketfd < 0)
    return -1;

  // Indicate (for debugging) that the server is running
  fprintf (stderr, "Server is started on port %s\n", protocol);

  // connection is the file descriptor for communicating with the current
  // client. This is distinct from the server socket (socketfd).
  int connection = get_connection (socketfd);
  if (connection < 0)
    {
      shutdown (socketfd, SHUT_RDWR);
      close (socketfd);
      return -1;
    }

  // TODO: Read the request from the socket into the buffer provided below.
  // If the call to read() fails, shutdown and close both file descriptors
  // and return NULL.
  char buffer[BUFFER_SIZE];
  memset (&buffer, 0, BUFFER_SIZE);
  ssize_t bytes = -1;

  bytes = read (connection, buffer, BUFFER_SIZE - 1);
  if (bytes <= 0)
    {
      shutdown (connection, SHUT_RDWR);
      close (connection);
      shutdown (socketfd, SHUT_RDWR);
      close (socketfd);
      return -1;
    }

  // TODO: Pass the buffer contents to build_response(). If build_response()
  // returns NULL, send a 404 back to the socket:
  //      "HTTP/1.0 404 Not Found\r\n\r\n";
  // Otherwise, just write the response back. If a 404 was sent, the return
  // value should be -1. Otherwise, return the number of bytes sent to the
  // socket.

  char *response = build_response (buffer);
  if (response == NULL)
    {
      // send a 404 back to the socket
      char *no_response = "HTTP/1.0 404 Not Found\r\n\r\n";
      write (connection, no_response, strlen (no_response));
      // shutdown and close both the connection and server sockets
      shutdown (connection, SHUT_RDWR);
      close (connection);
      shutdown (socketfd, SHUT_RDWR);
      close (socketfd);
      return -1;
    }

  // write response back to the socket
  bytes = write (connection, response, strlen (response));

  // TODO: Shutdown and close both the connection and server sockets.
  shutdown (connection, SHUT_RDWR);
  close (connection);
  shutdown (socketfd, SHUT_RDWR);
  close (socketfd);

  free (response);
  return bytes;
}
