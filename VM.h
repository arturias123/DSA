#ifndef VM_H
#define VM_H

#include "main.h"


class VM
{
public:
  VM(){}
  string ins[65536];
  int i = 0;
  void run(string filename);
};
#endif