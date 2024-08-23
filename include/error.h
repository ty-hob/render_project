#ifndef ERROR_H
#define ERROR_H

// struct for holding error information
typedef struct ERROR {
  char* message;
} error;

// creates a new error with the given message. the caller is responsible for
// freeing the memory
error* new_error(const char* message);

// frees the memory used by the error
void free_error(error* err);

#endif
