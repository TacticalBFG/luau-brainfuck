#include "serializer.h"

void writeB(std::ostringstream& ss, unsigned char value)
{
	ss.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

// straight outta sentinel!
void writeCInt(std::ostringstream& ss, unsigned int value)
{
	do
	{
		int v = value & 0x7F;
		value >>= 7;

		if (value)
			v |= 0x80;

		writeB(ss, v);
	} while (value);
}

void writeInteger(std::ostringstream& ss, int value)
{
	ss.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

std::string brainfuckTranslator(const char *s) {
	std::ostringstream ss;
	std::vector<std::string> constants;
	std::vector<Instruction> code;

	int reg = 0;

	char *cells = new char[255];
	char ptr = 0;
	for (int c = 0; c < 255; c++)
		cells[c] = 0;

	int nOpenedBrackets = 0;
	int nLoop = 0;
	char savedPc = 0;

	std::vector<jumpInfo*> jumps;
	std::string outputShit = "";

	code.push_back(generateABC(0xa3, 0,0, 0));

	for (int i = 0; i < strlen(s); i++) {
		char token = s[i];

		switch (token) {
		case '+':
			cells[ptr]++;
			break;
		case '-':
			cells[ptr]--;
			break;
		case '>':
			ptr++;
			break;
		case '<':
			ptr--;
			break;
		case '.': {
			std::string cellVal(1, cells[ptr]);
			outputShit.append(cellVal);
			break;
		}
		case '[': {
			int repetitions = cells[ptr];
			if (repetitions <= 1)
				break;
			nOpenedBrackets++;
			nLoop++;

			std::vector<char> tokensInside;
			int nCurrentBrackets = nOpenedBrackets;
			for (int tIdx = i + 1; tIdx < strlen(s); tIdx++) {
				char token = s[tIdx];


				if (token == ']') {
					nOpenedBrackets--;
					savedPc = tIdx;
					jumpInfo *j = new jumpInfo;
					j->from = tIdx;
					j->to = i;
					jumps.push_back(j);
					break;
				}

				tokensInside.push_back(token);
			}
			goto LOOP_EVAL;
			break;  }
		case ']': {
			goto LOOP_EVAL;
			break;
		}
		}
		LOOP_EVAL:
		for (auto jmp : jumps) {
			if (i == jmp->from) {
				i = jmp->to - 1;
				jmp->from = INT_MAX - 2; // what
				delete[] jmp;

			}
		}
	}

	reg++;
	constants.push_back(outputShit);
	constants.push_back("print");
	code.push_back(generateABx(0x35, reg, 0));
	code.push_back(constants.size() - 1);
	code.push_back(generateABx(0x6F, reg + 1, constants.size() - 2));
	code.push_back(generateABC(0x9F, reg, 2, 1));

	code.push_back(generateABC(0x82, 0, 1, 0));

	writeCInt(ss, 1);

	writeCInt(ss, constants.size());

	for (auto str : constants) {
		writeCInt(ss, str.length());
		ss.write(str.c_str(), str.length());
	}

	writeCInt(ss, 1);

	writeB(ss, reg*2);
	writeB(ss, 0);
	writeB(ss, 0);
	writeB(ss, 0);

	writeCInt(ss, code.size());
	for (int i = 0; i < code.size(); i++)
		writeInteger(ss, code.at(i));
	writeCInt(ss, constants.size());
	for (int i = 0; i < constants.size(); i++) {
		writeB(ss, 3);
		writeCInt(ss, i+1);
	}
	writeCInt(ss, 0);
	writeCInt(ss, 0);
	writeCInt(ss, 0);
	writeCInt(ss, 0);

	writeCInt(ss, 0);

	return ss.str();
}

// to execute:
// deserialize returned string
// spawn
// profit?
