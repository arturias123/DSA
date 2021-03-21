#ifndef VM_H
#define VM_H

#include "main.h"


class VM
{
public:
	VM() {
		fill_n(R, 16, "0");
	}
  string ins[65536];
  string R[16];
  int i = 0;
  void run(string filename);
  void calculate(string input);
  void compare(string input);
};
#endif