#include "VM.h"
// regex for instruction
regex twoOps("(\\w+)\\s(\\w+),\\s(\\S+)$");
regex oneOp("(\\w+)\\s(\\w+)$");
regex noOp("(\\w+)$");
regex arith("(Add|Minus|Div|Mul)\\sR([1-9]|1[0-5]),\\s(\\d+|\\d+[.]\\d+)$");
regex arithReg("(Add|Minus|Div|Mul)\\sR([1-9]|1[0-5]),\\sR([1-9]|1[0-5])$");
regex cmp("Cmp(EQ|NE|LT|LE|GT|GE)\\sR([1-9]|1[0-5]),\\s(\\S+)$");
regex log1("(And|Or)\\sR([1-9]|1[0-5]),\\s(\\S+)$");
regex nein("Not\\sR([1-9]|1[0-5])$");
regex ls("(Move|Load|Store)\\sR([1-9]|1[0-5]),\\s(\\S+)$");
regex seq1("Return|Halt$");
regex seq2("(Jump|Call)\\s(\\S+)$");
regex seq3("JumpIf\sR([1-9]|1[0-5]),\\s(\\S+)$");
regex boolean("true|false");
regex address("(\\d+[A]$");
regex integer("\\d+");
regex flOat("\\d+[.]\\d+");


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
				calculate(ins[i]);
			}
			else if (regex_match(ins[i], arithReg)) {
				calculate(ins[i]);
			}
			else if (regex_match(ins[i], cmp)) {
				compare(ins[i]);
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

void VM::calculate(string input) {
	smatch s;
	string a, b;
	int dest, src;
	bool ints = 0;
	if (regex_match(input, s, arith)) {
		a = R[stoi(s[2])];	// register ID
		b = s[3];			// value
	}
	else if (regex_match(input, s, arithReg)) {
		a = R[stoi(s[2])];	
		b = R[stoi(s[3])];	// register ID
	}
	if (regex_match(a, integer) && regex_match(b, integer)) {
		ints = 1;
	}
	else ints = 0;
	if (regex_match(a, address) || regex_match(a, boolean) || regex_match(b, address) || regex_match(b, boolean)) {
		throw TypeMismatch(i);
	}
	if (s[1] == "Add") {
		if (ints == 1) {
			R[stoi(s[2])] = to_string(stoi(a) + stoi(b));
		}
		else {
			R[stoi(s[2])] = to_string(stod(a) + stod(b)); // assign result to register
		}
	}
	else if (s[1] == "Minus") {
		if (ints == 1) {
			R[stoi(s[2])] = to_string(stoi(a) - stoi(b));
		}
		else {
			R[stoi(s[2])] = to_string(stod(a) - stod(b)); 
		}
	}
	else if (s[1] == "Div") {
		if (stod(b) == 0) {
			throw DivideByZero(i);
		}
		else {
			if (ints == 1) {
				R[stoi(s[2])] = to_string(stoi(a) / stoi(b));
			}
			else {
				R[stoi(s[2])] = to_string(stod(a) / stod(b));
			}
		}
	}
	else if (s[1] == "Mul") {
		if (ints == 1) {
			R[stoi(s[2])] = to_string(stoi(a) * stoi(b));
		}
		else {
			R[stoi(s[2])] = to_string(stod(a) * stod(b));
		}
	}
}

void VM::compare(string input) {
	smatch s;
	bool res = 0;
	string a, b;
	if (regex_match(input, s, cmp)) {
		a = R[stoi(s[2])]; // dest
		b = s[3];		   // src
	}
	if (regex_match(a, address) || regex_match(b, address)) {
		throw TypeMismatch(i);
	}
	if (regex_match(a, boolean) && !regex_match(b, boolean)) {
		throw TypeMismatch(i);
	}
	if (regex_match(b, boolean) && !regex_match(a, boolean)) {
		throw TypeMismatch(i);
	}
	if (s[1] == "EQ") {
		res = (a == b);
	}
	else if (s[1] == "NE") {
		res = (a != b);
	}
	else if (s[1] == "LT") {
		res = (a < b);
	}
	else if (s[1] == "LE") {
		res = (a <= b);
	}
	else if (s[1] == "GT") {
		res = (a > b);
	}
	else if (s[1] == "GE") {
		res = (a >= b);
	}
	if (res == 1) {
		R[stoi(s[2])] = "true";
	}
	else {
		R[stoi(s[2])] = "false";
	}
}

//stod(s[3])
//stod(R[stoi(s[3]])