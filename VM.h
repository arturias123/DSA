#ifndef VM_H
#define VM_H
//#define _REGEX_MAX_STACK_COUNT 1000
#include "main.h"


class VM
{
public:
	VM() {
		fill_n(R, 16, '0');
	}
  string ins[65536];
  string staticMem[65536];
  string R[16];
  int i = 0;
  void run(string filename);
  void calculate(string input);
  void compare(string input);
  void logic(string input);
  void removeTrailingZeros(string &input);
  void loadStore(string input);
  bool stob(string input);
  void sequence(string input);
};
#endif