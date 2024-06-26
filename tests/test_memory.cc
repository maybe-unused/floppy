#include <gtest/gtest.h>
#include <floppy/floppy.h>

struct A
{
  int a = 5;
  int b = 42;
  virtual auto name() -> std::string { return "A"; }
};

struct B : public A
{
  int c = 3;
  int d = 7;
  virtual auto name() -> std::string { return "B"; }
};

