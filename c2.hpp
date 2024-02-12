#pragma once
#include <cassert>
#include <execinfo.h>
#include <stdexcept>

inline void print_backtrace()
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

template<typename To, typename From>
inline bool isa(const From* Val)
{
  return To::classof(Val);
}

template<typename To>
inline bool isa(const To* Val)
{
  return true;
}

template<typename To, typename From>
inline To* cast(From* Val)
{
  // assert(isa<To>(Val) && "Invalid cast!");
  if (!isa<To>(Val)) {
    print_backtrace();
    throw std::runtime_error("Invalid cast!");
  }
  return static_cast<To*>(Val);
}

template<typename To>
inline To* cast(To& Val)
{
  return Val;
}

template<typename To, typename From>
inline To* dyn_cast(From* Val)
{
  return isa<To>(Val) ? static_cast<To*>(Val) : nullptr;
}

template<typename To>
inline To* dyn_cast(To& Val)
{
  return Val;
}
