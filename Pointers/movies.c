#define _POSIX_C_SOURCE 200809L // needed for strdup extension

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "movies.h"

// Given a string (possibly read-only) of IMDB movie data
// (formatted as "Title,Rating,Runtime,Year,Genre,OscarNominated"),
// create a movie_t object based on the appropriate fields.
movie_t
split_data (char *csv)
{
  movie_t movie;
  char *csv_copy = strdup (csv);
  char *token = strtok (csv_copy, ",");
  movie.title = strdup (token); // make a copy the first token as the title

  // Use strtok() repeatedly to get each string one at a time and format
  // the data fields as follows:

  //   Use strtol() on the next token as year
  char *year_token = strtok (NULL, ",");
  long year = strtol (year_token, NULL, 10);
  movie.year = year;

  //   Copy the token as the movie's genre field
  char *genre_token = strtok (NULL, ",");
  movie.genre
      = strdup (genre_token); // make a copy of the third token as the genre

  // Free up any unnecessary allocated memory. This must NOT include the
  // movie object or any duplicated strings it points to.
  free (csv_copy);
  csv_copy = NULL;

  return movie;
}

// Build a dynamically allocated string from a movie_t object as follows:
// "Title [Year] - Genre"
// Rather than allocating all of the space ahead of time, use realloc()
// to grow the string at different stages as needed.
char *
merge_data (movie_t movie)
{
  // Initially, just make a duplicate of the title for the given movie.
  char *movie_copy = strdup (movie.title);

  // HINT: When doing string manipulation, you should use a variable to keep
  // explicit track of the string length. C string lengths are not inherently
  // stored anywhere.
  size_t len = 1; // always keep track of the null byte
  char *result = calloc (len, sizeof (char));

  // Now, use realloc() to grow the string to have space for the rest. You
  // should NOT rely on any length unless its exact value is known. For
  // instance, you can assume that " [Year] - " adds 10 bytes, but you cannot
  // assume anything about the length of the genre. As such, you need to
  // compute the new length using strlen().
  len += strlen (movie_copy) + 10
         + strlen (movie.genre); // assume the year field is 10 bytes in length
  result = realloc (result, len * sizeof (char));

  // Finally, concatenate the " [Year] - Genre" values after the title. The
  // most straightforward way to do this is with a single call to snprintf(),
  // which behaves VERY similarly to printf(). You could also use strncat(),
  // but that is actually more work than it is worth.
  snprintf (result, len, "%s [%d] - %s", movie.title, movie.year, movie.genre);

  free (movie_copy);
  movie_copy = NULL;

  return result;
}
