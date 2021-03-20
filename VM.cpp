#include "VM.h"
// regex for instruction
regex twoOps("(\\w+)\\s(\\w+),\\s(\\S+)$");
regex oneOp("(\\w+)\\s(\\w+)");
regex noOp("(\\w+)");
regex arith("(Add|Minus|Div|Mul)\\sR([1-9]|1[0-5]),\\s(\\S+)");
regex cmp("Cmp(EQ|NE|LT|LE|GT|GE)\\sR([1-9]|1[0-5]),\\s(\\S+)");
regex log1("(And|Or)\\sR([1-9]|1[0-5]),\\s(\\S+)");
regex nein("Not\\sR([1-9]|1[0-5])");
regex ls("(Move|Load|Store)\\sR([1-9]|1[0-5]),\\s(\\S+)");
regex seq1("Return|Halt");
regex seq2("(Jump|Call)\\s(\\S+)");
regex seq3("JumpIf\sR([1-9]|1[0-5]),\\s(\\S+)");

void VM::run(string filename)
{
	string myText;

	// Read from the text file
	ifstream myReadFile;
	myReadFile.open(filename);

	// Use a while loop together with the getline() function to read the file line by line
	while (getline(myReadFile, myText)) {
		// Output the text from the file
		ins[i] = myText;
		if (regex_match(ins[i], twoOps)) {
			if (regex_match(ins[i], arith)) {

			}
			else if (regex_match(ins[i], cmp)) {

			}
			else if (regex_match(ins[i], log1)) {

			}
			else if (regex_match(ins[i], ls)) {

			}
			else if (regex_match(ins[i], seq3)) {

			}
			else throw InvalidOperand(i);

		}
		else if (regex_match(ins[i], oneOp)) {
			if (regex_match(ins[i], nein)) {

			}
			else if (regex_match(ins[i], seq2)) {

			}
			else throw InvalidOperand(i);
		}
		else if (regex_match(ins[i], noOp)) {
			if (regex_match(ins[i], seq1)) {

			}
			else throw InvalidOperand(i);
		}
		else throw InvalidInstruction(i);
		i++;
	}

	// Close the file
	myReadFile.close();
}