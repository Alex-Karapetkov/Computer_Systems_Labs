#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

static char *build_response_header (size_t);
static char *get_uri (char *);

/* Helper function for configuring an IPv4 address. Takes an IP address
   and a port number (as a char*) and sets the appropriate fields in the
   struct sockaddr_in.
 */
struct sockaddr_in
build_address (const uint32_t ip, const char *protocol)
{
  // TODO: Manually configure a struct sockaddr_in to use the IP address
  // and port number (protocol) given. The port number is provided as a
  // string, so you need to use strtol() to convert it to a number. You
  // also need to indicate that this is an IPv4 address and to make sure
  // you have the correct endianness (using htonl() and htons()).
  //
  // HINT: The IP address must be set into the sin_addr.s_addr field, not
  // just the sin_addr field.

  struct sockaddr_in localhost;
  memset (&localhost, 0, sizeof (localhost));
  localhost.sin_family = AF_INET; // IPv4 address
  long port_num = strtol (protocol, NULL, 10);
  localhost.sin_port = htons (port_num);  // port number
  localhost.sin_addr.s_addr = htonl (ip); // set IP address

  struct addrinfo server;
  memset (&server, 0, sizeof (server));
  server.ai_family = AF_INET;          // grab IPv4 only
  server.ai_socktype = SOCK_STREAM;    // specify byte-streaming
  server.ai_flags = AI_PASSIVE;        // use default IP address
  server.ai_protocol = IPPROTO_TCP;    // create as a TCP socket
  server.ai_addrlen = INET_ADDRSTRLEN; // IPv4 address length
  server.ai_addr = (struct sockaddr *)&localhost;

  return localhost;
}

/* Given the specified protocol argument (can be a port number turned into a
   string or an application-layer protocol name), find the list of server
   addrinfo instances provided by getaddrinfo(). For this lab, only use TCP
   over IPv4. Loop through the list to create and bind a socket. If everything
   is successful, use listen() to convert the socket to a server socket and
   return the file descriptor. Return -1 if any error occurs.
 */
int
setup_server (const char *protocol)
{
  // Create an IPv4 TCP socket, then conigure it with the SO_REUSEADDR
  // and SO_RCVTIMEO socket options. Use 5 seconds for the timeout. Note
  // that the struct timeval type is an array of two integer values. The
  // first is the number of seconds and the second is the number of
  // milliseconds. So 5 seconds would be { 5, 0 }.
  int socketfd = -1;
  socketfd = socket (AF_INET, SOCK_STREAM, 0);
  assert (socketfd >= 0);
  int sockopt = 1;
  setsockopt (socketfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&sockopt,
              sizeof (int));
  struct timeval timeout = { 5, 0 };
  setsockopt (socketfd, SOL_SOCKET, SO_RCVTIMEO, (const void *)&timeout,
              sizeof (timeout));

  // Now, get the localhost address information and bind the socket to it.
  // Note that 0x7f000001 is the numeric version of 127.0.0.1. If the bind()
  // fails, close the socket and return -1.
  struct sockaddr_in localhost = build_address (0x7f000001, protocol);

  if (bind (socketfd, (struct sockaddr *)&localhost, INET_ADDRSTRLEN) != 0)
    {
      close (socketfd);
      return -1;
    }

  // Finally, use listen() to convert it to a server socket that can handle
  // a backlog of 10 pending connections before returning the socket.

  listen (socketfd, 10);
  return socketfd;
}

/* Wait for an incoming connection to arrive. When it does, use inet_ntoa()
   to print the string version of the IP address (use address.sin_addr).
   Return the connection socket file descriptor. If accept() returns a
   negative value, print the error message and return -1.

   NOTE: inet_ntoa() (in contrast to inet_ntop()) returns a pointer to a
   statically allocated buffer, so the return value should NOT be freed.
 */
int
get_connection (int socketfd)
{
  // For this lab, only use IPv4 addresses:
  struct sockaddr_in address;
  memset (&address, 0, sizeof (address));
  socklen_t addresslen = sizeof (struct sockaddr);

  // TODO: Accept the connection request and return the specified values
  // described above.

  int connection = accept (socketfd, (struct sockaddr *)&address, &addresslen);
  if (connection < 0)
    {
      fprintf (stderr, "Connection failed: %d\n", errno);
      return -1;
    }

  printf ("Received incoming request from %s\n", inet_ntoa (address.sin_addr));
  return connection;
}

/* Build the HTTP response for the requested URI. Use build_response_header()
   to get the HTTP header based on the file size. Then, allocate enough space
   for both the header and the file contents. Concatenate them and return
   the result.

   Example: For index.html, the returned result should look be
   (ignore spaces at the beginning of the lines):

      HTTP/1.0 200 OK\r\n
      Content-Type: text/html; charset=UTF-8\r\n
      Content-Length: 119\r\n
      \r\n
      <html>
        <head>
          <title>Success!</title>
        </head>

        <body>
          <h2>It <i>really</i> works!</h2>
        </body>
      </html>

   HINT: You do NOT need to parse through the HTML contents in any way. All
   you need to do is to determine the file size (using fstat()), allocate
   that much space, then read the contents of the file. Once you have both
   the headers and the file contents, concatenate both into a single string.
 */
char *
build_response (char *buffer)
{
  // Start by parsing the buffer to make sure that this is a GET request and
  // determine the URI (i.e., the file name). If anything goes wrong, return
  // NULL.
  char *uri = get_uri (buffer);
  if (uri == NULL)
    {
      printf ("Failed to get URI\n");
      return NULL;
    }
  printf ("GET request for %s\n", uri);

  // Next, open the file for reading and get the file size. Pass that to
  // build_response_header(), then concatenate the file contents.

  struct stat info;
  int fd = open (uri, O_RDONLY);

  if (fd <= 0)
    {
      free (uri);
      close (fd);
      return NULL;
    }

  if (fstat (fd, &info) < 0)
    {
      free (uri);
      close (fd);
      return NULL;
    }

  // determine file size and pass to build_response_header()
  size_t file_size = info.st_size;
  char *response_header = build_response_header (file_size);

  // allocate space for header and file contents
  size_t length = strlen (response_header) + file_size + 1;
  char *HTTP_response = calloc (length, sizeof (char));
  // copy response header at start of allocated string
  strncpy (HTTP_response, response_header, length);

  // read file contents
  char *file_contents = calloc (file_size, sizeof (char));
  ssize_t bytes = read (fd, file_contents, file_size);
  if (bytes != file_size)
    {
      free (uri);
      free (response_header);
      free (HTTP_response);
      free (file_contents);
      close (fd);
      return NULL;
    }

  // concatenate file contents to header
  strncat (HTTP_response, file_contents, length - strlen (HTTP_response));
  free (response_header);
  close (fd);
  free (uri);
  return HTTP_response;
}

// HELPER FUNCTIONS BELOW (no code to implement)

/* Build an HTTP response header based on the version and length.
   Recall that all lines must end with \r\n, and there must be an
   additional \r\n to indicate a blank line at the end. For example,
   if length is 25, the response header should be (all as a single
   string):

     "HTTP/1.0 200 OK\r\n
      Content-Type: text/html; charset=UTF-8\r\n
      Content-Length: 25\r\n\r\n"

   For simplicity, we assume that this header string is less than
   100 bytes in length.

   The return value should be a dynamically allocated char* that will
   be freed by the caller.
 */
static char *
build_response_header (size_t length)
{
  char *headers = calloc (100, sizeof (char));

  snprintf (headers, 99,
            "HTTP/1.0 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n"
            "Content-Length: %zu\r\n\r\n",
            length);
  return headers;
}

/* Given the contents of a buffer, split the request to get the first
   line (ending in \r\n). Make sure the request type is "GET" (return
   NULL for other types of request). For a GET request, return the name
   of the requested URI. If the URI is just '/', return "index.html".
 */
static char *
get_uri (char *buffer)
{
  char *token = strtok (buffer, " ");
  if (strncmp (token, "GET", 4))
    return NULL;

  token = strtok (NULL, " ");
  if (token == NULL)
    return NULL;
  if (strlen (token) < 1) // Should never happen
    return NULL;
  char *uri = strdup (token);
  if (uri[0] == '/')
    {
      uri = realloc (uri, 11);
      assert (uri != NULL);
      strncpy (uri, "index.html", 11);
    }

  return uri;
}
