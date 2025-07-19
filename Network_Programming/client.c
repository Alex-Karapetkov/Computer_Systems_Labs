#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "client.h"

#define BUFFER_MAX_SIZE 4096

/* Given a struct with server information, return a dynamically allocated
   string of the IP address in dotted decimal or colon hexadecimal notation.
   Consult Chapter 4, as well as /usr/include/netdb.h and
   /usr/include/arpa/inet.h as needed. Use inet_ntop() to get the formatted
   string based on the address's ai_addr field.
 */
char *
addr_string (struct addrinfo *server)
{
  // Return safely in case server is NULL:
  if (server == NULL)
    return strdup ("no address information");

  // define empty return string
  char *addr_string = NULL;

  // if IP address is IPv4, return dotted decimal notation
  if (server->ai_family == AF_INET)
    {
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)server->ai_addr;
      // allocate memory for IPv4 address string based on INET_ADDRSTRLEN
      addr_string = malloc (INET_ADDRSTRLEN);

      // use inet_ntop() to convert IP address to string format
      inet_ntop (AF_INET, &ipv4->sin_addr, addr_string, INET_ADDRSTRLEN);
    }
  else if (server->ai_family == AF_INET6)
    {
      // cast ai_addr to an IPv6 socket address
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)server->ai_addr;

      // allocate memory for IPv6 address string based on INET6_ADDRSTRLEN
      addr_string = malloc (INET6_ADDRSTRLEN);

      inet_ntop (AF_INET6, &ipv6->sin6_addr, addr_string, INET6_ADDRSTRLEN);
    }

  return addr_string;
}

/* Given the server address info, return a dynamically allocated string with
   its transport-layer protocol, IP version, and IP address. For instance,
   querying jmu.edu over TCP/IPv4 should return:
     "TCP IPv4 134.126.10.50"
   Use addr_string() to get the formatted address string, concatenate it to
   the string to return, then free the result from addr_string(). If the
   passed server parameter is NULL, use strdup() to return a dynamically
   allocated copy of "no address information".

   NOTE: When distinguishing TCP and UDP, you should be checking the
   server->ai_socktype field, not server->ai_protocol. The test cases do not
   set the server->ai_protocol, which is sometimes not used in practice.
 */
char *
serv_string (struct addrinfo *server)
{
  // Return safely in case server is NULL:
  if (server == NULL)
    return strdup ("no address information");

  char *serv_string = NULL;

  // Allocate 10 bytes for protocol (TCP or UDP), IP version (IPv4 or
  // IPv6), along with spaces and the ending null byte. In addition,
  // include enough space for an IP address (either v4 or v6).
  size_t buffer_length;
  if (server->ai_family == AF_INET)
    {
      buffer_length = 10 + INET_ADDRSTRLEN;
    }
  else
    {
      buffer_length = 10 + INET6_ADDRSTRLEN;
    }

  serv_string = malloc (buffer_length);
  // Now, use strncat() to build the string based on information from
  // the server struct.

  // concatenate transport layer protocol based on server's socktype
  if (server->ai_socktype == SOCK_STREAM)
    {
      strncpy (serv_string, "TCP ", buffer_length);
    }
  else if (server->ai_socktype == SOCK_DGRAM)
    {
      strncpy (serv_string, "UDP ", buffer_length);
    }

  // concatenate IP version based on server's ai_family
  if (server->ai_family == AF_INET)
    {
      strncat (serv_string, "IPv4 ", buffer_length);
    }
  else
    {
      strncat (serv_string, "IPv6 ", buffer_length);
    }

  // concatenate IP address
  char *address_string = addr_string (server);
  strncat (serv_string, address_string, strlen (address_string));

  free (address_string);
  return serv_string;
}

/* Given a hostname string, use getaddrinfo() to query DNS for the specified
   protocol parameters. Boolean values indicate whether or not to use IPv6
   (as opposed to IPv4) or TCP (as opposed to UDP). For this lab, the protocol
   will only be "http", though it could be others in a more complete
   implementation. Return the pointer to the linked list of server results.
 */
struct addrinfo *
get_server_list (const char *hostname, const char *proto, bool tcp, bool ipv6)
{
  struct addrinfo *server_list = NULL;
  struct addrinfo hints;
  memset (&hints, 0, sizeof (hints));

  if (tcp)
    {
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = IPPROTO_TCP;
    }
  else
    {
      hints.ai_socktype = SOCK_DGRAM;
      hints.ai_protocol = IPPROTO_UDP;
    }

  if (ipv6)
    {
      hints.ai_family = AF_INET6;
    }
  else
    {
      hints.ai_family = AF_INET;
    }

  // error checking
  int check_status = getaddrinfo (hostname, proto, &hints, &server_list);
  if (check_status != 0)
    {
      freeaddrinfo (server_list);
      return NULL;
    }

  return server_list;
}

/* OPTIONAL: You do not need to complete the next function, but you are
   encouraged to do so. It will give you a head start on the project work.

   Use an HTTP/1.0 request to retrieve the requested file with a GET request.
   Return the first line of the header and the content type. To get the type,
   use strtok() to split the lines of the header at "\n". Then look for the
   line that beging "Content-Type: ". Set the call-by-reference paramter type
   to be the remainder of that line. Be sure to trim off the "\r" at the end.
 */
char *
web (const char *hostname, char *proto, const char *file, char **type,
     bool ipv6)
{
  // To start, call get_server_list(), then loop through the results trying
  // to create a socket connection.

  // call get_server_list()
  struct addrinfo *server_info = get_server_list (hostname, proto, true, ipv6);

  // loop through results from get_server_list () to create a socket connection
  struct addrinfo *server = NULL;
  int socketfd = -1;
  for (server = server_info; server != NULL; server = server->ai_next)
    {
      // attempt to create TCP socket
      if ((socketfd = socket (server->ai_family, server->ai_socktype,
                              server->ai_protocol))
          < 0)
        continue;
      if (connect (socketfd, server->ai_addr, server->ai_addrlen) == 0)
        break;
      close (socketfd);
      socketfd = -1;
    }

  freeaddrinfo (server_info);
  if (socketfd < 0)
    return NULL;
  // begin HTTP session

  // When a connection is successfully
  // established, send "GET file HTTP/1.0\r\n\r\n" for the file specified.
  // Write the request to the socket and read the result into the buffer.

  char buffer[BUFFER_MAX_SIZE]; // Should not need more that this
  memset (buffer, 0, sizeof (buffer));
  strncpy (buffer, "GET ", BUFFER_MAX_SIZE);
  strncat (buffer, file, BUFFER_MAX_SIZE - strlen (buffer));
  strncat (buffer, " HTTP/1.0\r\n\r\n", BUFFER_MAX_SIZE - strlen (buffer));

  // write request to socket
  write (socketfd, buffer, strlen (buffer));

  // read response into buffer
  ssize_t bytes = read (socketfd, buffer, BUFFER_MAX_SIZE);
  assert (bytes > 0);

  // Once the response is read into the buffer, uncomment this code:
  char *token = strtok (buffer, "\n");
  char *result = calloc (strlen (token), sizeof (char));
  strncpy (result, token, strlen (token) - 1);

  while ((token = strtok (NULL, "\n")) != NULL)
    {
      if (!strncmp (token, "Content-Type: ", 14))
        {
          *type = calloc (strlen (token) - 14, sizeof (char));
          strncpy (*type, token + 14, strlen (token) - 15);
          return result;
        }
    }
  return result;
}
