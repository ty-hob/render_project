#include <stdlib.h>
#include <string.h>

#include "rendeng/error.h"

error* new_error(const char* message) {
  error* e = malloc(sizeof(error));
  if (e == NULL) {
    return NULL;
  }

  e->message = calloc(strlen(message) + 1, sizeof(char));
  if (e->message == NULL) {
    free(e);

    return NULL;
  }

  strcpy(e->message, message);

  return e;
}

void free_error(error* e) {
  if (e->message != NULL) {
    free(e->message);
  }

  free(e);
}
