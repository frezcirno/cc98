#include "c2.hpp"
#include <cassert>
#include <execinfo.h>

void print_backtrace()
{
  const int max_frames = 64;
  void* array[max_frames];
  size_t size = backtrace(array, max_frames);

  fprintf(stderr, "Obtained %zd stack frames.\n", size);

  char** symbols = backtrace_symbols(array, size);
  if (symbols == nullptr) {
    perror("backtrace_symbols");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < size; ++i) {
    fprintf(stderr, "%zu: %s\n", i, symbols[i]);
  }

  free(symbols);
}
