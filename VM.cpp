#include "VM.h"
// regex for instruction
regex twoOps("^(\\w+)\\s(\\w+),\\s(\\S+)$");
regex oneOp("^(\\w+)\\s(\\w+)$");
regex noOp("^(\\w+)$");
regex arith("(Add|Minus|Div|Mul)\\sR([1-9]|1[0-5]),\\s(\\d+|\\d+[.]\\d+)$");
regex arithReg("(Add|Minus|Div|Mul)\\sR([1-9]|1[0-5]),\\sR([1-9]|1[0-5])$");
regex cmp("Cmp(EQ|NE|LT|LE|GT|GE)\\sR([1-9]|1[0-5]),\\s(\\d+|\\d+[.]\\d+)$");
regex cmpReg("Cmp(EQ|NE|LT|LE|GT|GE)\\sR([1-9]|1[0-5]),\\sR([1-9]|1[0-5])$");
regex log1("(And|Or)\\sR([1-9]|1[0-5]),\\s(\\S+)$");
regex logReg("(And|Or)\\sR([1-9]|1[0-5]),\\sR([1-9]|1[0-5])$");
regex nein("(Not)\\sR([1-9]|1[0-5])$");
regex ls("(Move|Load|Store)\\sR([1-9]|1[0-5]),\\s(\\S+)$");
regex seq1("(Return|Halt)$");
regex seq2("(Jump|Call)\\s(\\S+)$");
regex seq3("JumpIf\\sR([1-9]|1[0-5]),\\s(\\S+)$");
regex boolean("true|false");
regex address("(\\d+)[A]$");
regex integer("\\d+");
regex flOat("\\d+[.]\\d+");
regex reg("R([1-9]|1[0-5])$");
regex io("^(Input|Output)\\s(\\S+)$");


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
		insCount++;
	}
	while (i < insCount) {
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
			else if (regex_match(ins[i], cmpReg)) {
				compare(ins[i]);
			}
			else if (regex_match(ins[i], log1)) {
				logic(ins[i]);
			}
			else if (regex_match(ins[i], logReg)) {
				logic(ins[i]);
			}
			else if (regex_match(ins[i], ls)) {
				loadStore(ins[i]);
			}
			else if (regex_match(ins[i], seq3)) {
				sequence(ins[i]);
			}
			else throw InvalidOperand(i);
		}
		else if (regex_match(ins[i], oneOp)) {
			if (regex_match(ins[i], nein)) {
				logic(ins[i]);
			}
			else if (regex_match(ins[i], seq2)) {
				sequence(ins[i]);
			}
			else if (regex_match(ins[i], io)) {
				inOut(ins[i]);
			}
			else throw InvalidOperand(i);
		}
		else if (regex_match(ins[i], noOp)) {
			if (regex_match(ins[i], seq1)) {
				sequence(ins[i]);
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
			R[stoi(s[2])] = to_string(stof(a) + stof(b)); // assign result to register
		}
	}
	else if (s[1] == "Minus") {
		if (ints == 1) {
			R[stoi(s[2])] = to_string(stoi(a) - stoi(b));
		}
		else {
			R[stoi(s[2])] = to_string(stof(a) - stof(b));
		}
	}
	else if (s[1] == "Div") {
		if (stod(b) == 0) {
			throw DivideByZero(i);
		}
		else {
			R[stoi(s[2])] = to_string(stod(a) / stod(b));
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
	if (regex_match(R[stoi(s[2])], flOat)) {
		removeTrailingZeros(R[stoi(s[2])]);
	}
	cout << R[stoi(s[2])] << endl;
}

void VM::compare(string input) {
	smatch s;
	bool res = 0;
	string a, b;
	if (regex_match(input, s, cmp)) {
		a = R[stoi(s[2])]; // dest
		b = s[3];		   // src
	}
	else if (regex_match(input, s, cmpReg)) {
		a = R[stoi(s[2])];
		b = R[stoi(s[3])];
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
	cout << R[stoi(s[2])] << endl;
}

void VM::logic(string input) {
	smatch s;
	bool dest, src;
	string dest2, src2, res;
	if (regex_match(input, s, log1)) {
		dest2 = R[stoi(s[2])];
		src2 = s[3];
		if (regex_match(dest2, boolean) && regex_match(src2, boolean)) {
			dest = stob(dest2);
			src = stob(src2);
		}
		else throw TypeMismatch(i);
		if (s[1] == "And") {
			dest = (dest && src);
		}
		else {
			dest = (dest || src);
		}
		if (dest == 0) {
			res = "false";
		}
		else {
			res = "true";
		}
		R[stoi(s[2])] = res;
	}
	else if (regex_match(input, s, logReg)) {
		dest2 = R[stoi(s[2])];
		src2 = R[stoi(s[3])];
		if (regex_match(dest2, boolean) && regex_match(src2, boolean)) {
			dest = stob(dest2);
			src = stob(src2);
		}
		else throw TypeMismatch(i);
		if (s[1] == "And") {
			dest = (dest && src);
		}
		else {
			dest = (dest || src);
		}
		if (dest == 0) {
			R[stoi(s[2])] = "false";
		}
		else {
			R[stoi(s[2])] = "true";
		}
	}
	else if (regex_match(input, s, nein)) {
		dest2 = R[stoi(s[2])];
		if (regex_match(dest2, boolean)) {
			dest = stob(dest2);
		}
		else throw TypeMismatch(i);
		dest = !dest;
		if (dest == 0) {
			res = "false";
		}
		else {
			res = "true";
		}
		R[stoi(s[2])] = res;
	}
}

void VM::removeTrailingZeros(string& input) {
	int i = input.length() - 1;
	while (input[i] == '0') {
		input.erase(i, 1);
		i--;
	}
	if (input[i] == '.') {
		input.append(i, '0');
	}
}

bool VM::stob(string input) {
	if (input == "true") {
		return true;
	}
	else return false;
}

void VM::loadStore(string input) {
	smatch s, m, add;
	string dest, s3, src;
	if (regex_match(input, s, ls)) {
		dest = R[stoi(s[2])];
		s3 = s[3];
		if (s[1] == "Move") {
			if (regex_match(s3, m, reg)) {
				src = R[stoi(m[1])];
				R[stoi(s[2])] = src;
			}
			else {
				R[stoi(s[2])] = s3;
			}
			cout << R[stoi(s[2])] << endl;
		}
		else if (s[1] == "Load") {
			if (regex_match(s3, m, reg)) {
				src = R[stoi(m[1])];
				if (regex_match(src, add, address)) {
					dest = staticMem[stoi(add[1])];
				}
				else throw TypeMismatch(i);
			}
			else {
				if (regex_match(s3, add, address)) {
					dest = staticMem[stoi(add[1])];
				}
				else throw TypeMismatch(i);
			}
			R[stoi(s[2])] = dest;
			cout << R[stoi(s[2])] << endl;
		}
		else if (s[1] == "Store") {
			if (regex_match(dest, add, address)) {
				if (regex_match(s3, m, reg)) {
					src = R[stoi(m[1])];
				}
				else {
					src = s3;
				}
				staticMem[stoi(add[1])] = src;
			}
			else throw TypeMismatch(i);
			cout << staticMem[stoi(add[1])] << endl;
		}
	}
}

void VM::sequence(string input) {
	smatch s, add;
	string dest, src;
	if (regex_match(input, s, seq1)) {
		string control = s[1];
		if (control == "Return") {
			if (sCount > 0 && sCount < MAX_STACK_COUNT) {
				R[0] = stack[--sCount];
				stack[sCount] == "";
			}
			else throw StackFull(i);
		}
		else if (control == "Halt") {
			i = insCount;
		}
	}
	else if (regex_match(input, s, seq2)) {
		if (s[1] == "Jump") {
			src = s[2];
			if (regex_match(src, add, address)) {
				R[0] = staticMem[stoi(add[1])];
			}
			else throw TypeMismatch(i);
		}
		else if (s[1] == "Call") {
			src = s[2];
			if (regex_match(src, add, address)) {
				// push value of IP onto stack
				stack[sCount++] = R[0];
				R[0] = staticMem[stoi(add[1])];
			}
			else throw TypeMismatch(i);
		}
	}
	else if (regex_match(input, s, seq3)) {
		dest = R[stoi(s[1])];
		src = s[2];
		if (regex_match(dest, boolean) && regex_match(src, add, address)) {
			R[0] = staticMem[stoi(add[1])];
		}
		else throw TypeMismatch(i);
	}
}

void VM::inOut(string input) {

}