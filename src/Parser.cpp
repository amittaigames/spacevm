#include <VM/Parser.h>

namespace VM {

    Parser::Parser(char* text, int flen) {
        this->text = text;
		this->flen = flen;
		this->tokenList = new TokenList(1);
		this->lblList = new List<char*>(1);
		this->jmpList = new List<char*>(1);
		this->varList = new List<char*>(1);
		this->addrList = new List<char*>(1);
    }

    Parser::~Parser() {
        delete[] this->text;
    }

    void Parser::start() {
        char* lex = new char[256];
        int lexi;
        int i = 0;
        char delim = ' ';
		int line = 1;

        // Reset the buffer
        resetLex:
        for (int x = 0; x < 256; x++)
            lex[x] = '\0';
		lexi = 0;

		// Ignore empty lines
		if (text[i] == '\n') {
			i++;
			line++;
			goto resetLex;
		}

        // Count up until delimeter is reached
        while (text[i] != '\n' && text[i] != '\0' &&
				text[i] != delim && i < this->flen) {
			if (text[i] == '\t') {
				i++;
				continue;
			}
			
			if (text[i] == ';') {
				while (text[i] != '\n' && text[i] != '\0')
					i++;
				continue;
			}
			
			lex[lexi++] = text[i++];
        }

        // Set the delimeter to nothing
        lex[lexi] = '\0';
		i++;
		
		if (nextPPI) {
			ppi(lex);
		}

		// REGISTERS
		int tokenData;	// used later
		
		if (lex[0] == '%') {
			tokenList->add(new Token(TokenType::REG, Token::getRegToken(lex)));
		}

		// NUMBERS
		else if (lex[0] == '$') {
			tokenList->add(new Token(TokenType::NUM, Token::convertNumber(lex, 10)));
		}
		else if (lex[0] == '0' && lex[1] == 'x') {
			tokenList->add(new Token(TokenType::NUM, Token::convertNumber(lex, 16)));
		}

		// LABELS
		else if (lex[0] == ':') {
			char* labelName = Util::strDup(lex, 1, Util::strLength(lex));
			// TODO: check for duplicates
			lblList->add(labelName);
			tokenList->add(new Token(TokenType::LBL, lblList->getPointer()-1));
		}
		else if (lex[0] == '@') {	
			char* jumpName = Util::strDup(lex, 1, Util::strLength(lex));
			jmpList->add(jumpName);
			tokenList->add(new Token(TokenType::JMP_T, jmpList->getPointer()-1));
		}
		
		// VARIABLES
		else if (lex[0] == '&') {
			char* addr = Util::strDup(lex, 1, Util::strLength(lex));
			addrList->add(addr);
			tokenList->add(new Token(TokenType::ADDR, addrList->getPointer()-1));
		}
		else if (lex[0] == '*') {
			char* var = Util::strDup(lex, 1, Util::strLength(lex));
			varList->add(var);
			tokenList->add(new Token(TokenType::VAR, varList->getPointer()-1));
		}
		
		// PREPROCESSORS
		else if (lex[0] == '#') {
			char* inst = Util::strDup(lex, 1, Util::strLength(lex));
			ppi(inst);
		}
		
		// INSTRUCTIONS
		else if ((tokenData = checkInst(lex)) != -1) {
			tokenList->add(new Token(TokenType::INST, tokenData));
		}

		// UNKNOWN
		else {
			std::cerr << "Syntax Error (" << line << ") : " << lex << std::endl;
			panic("Aborting");
		}

		if (i >= flen)
			return;

		// Line for debugging
		if (text[i-1] == '\n')
			line++;

		// Check for EOF
		if (text[i-1] != '\0')
			goto resetLex;
    }

	void Parser::showTokenList() {
		this->tokenList->showList();
	}
	
	void Parser::showLabelLists() {
		if (this->lblList->getPointer() > 0) {
			std::cout << "Label List: ";
			this->lblList->showList();
		}
		
		if (this->jmpList->getPointer() > 0) {
			std::cout << "Jump List: ";
			this->jmpList->showList();
		}
	}

	Compiler* Parser::createCompiler(const char* path) {
		Compiler* c = new Compiler(path, this->tokenList);
		c->setLabelList(this->lblList);
		c->setJumpList(this->jmpList);
		c->setAddrList(this->addrList);
		c->setVarList(this->varList);
		return c;
	}
	
	void Parser::ppi(char* inst) {
		if (!nextPPI) {
			if (Util::strEquals(inst, "data")) {
				tokenList->add(new Token(TokenType::PPI, TokenPPI::DATA));
			} else if (Util::strEquals(inst, "end")) {
				tokenList->add(new Token(TokenType::PPI, TokenPPI::END));
			} else {
				std::cerr << "Invalid Preprocessor: '" << inst << "'" << std::endl;
				panic("Aborting");
			}
		}
	}

	int Parser::checkInst(char* lex) {
		if (Util::strEquals(lex, "mov"))
            return TokenInst::MOV;
		else if (Util::strEquals(lex, "add"))
			return TokenInst::ADD;
		else if (Util::strEquals(lex, "sub"))
			return TokenInst::SUB;
		else if (Util::strEquals(lex, "call"))
			return TokenInst::CALL;
		else if (Util::strEquals(lex, "ret"))
			return TokenInst::RET;
		else if (Util::strEquals(lex, "sysi"))
			return TokenInst::SYSI;
		else if (Util::strEquals(lex, "mul"))
			return TokenInst::MUL;
		else if (Util::strEquals(lex, "div"))
			return TokenInst::DIV;
		else if (Util::strEquals(lex, "sxr"))
			return TokenInst::SXR;
		else if (Util::strEquals(lex, "sxl"))
			return TokenInst::SXL;
		else if (Util::strEquals(lex, "inc"))
			return TokenInst::INC;
		else if (Util::strEquals(lex, "dec"))
			return TokenInst::DEC;
		else if (Util::strEquals(lex, "push"))
			return TokenInst::PUSH;
		else if (Util::strEquals(lex, "pop"))
			return TokenInst::POP;
		else if (Util::strEquals(lex, "cmp"))
			return TokenInst::CMP;
		else if (Util::strEquals(lex, "jmp"))
			return TokenInst::JMP;
		else if (Util::strEquals(lex, "jne"))
			return TokenInst::JNE;
		else if (Util::strEquals(lex, "je"))
			return TokenInst::JE;
		else if (Util::strEquals(lex, "jg"))
			return TokenInst::JG;
		else if (Util::strEquals(lex, "jl"))
			return TokenInst::JL;
		else if (Util::strEquals(lex, "jge"))
			return TokenInst::JGE;
		else if (Util::strEquals(lex, "jle"))
			return TokenInst::JLE;
		else if (Util::strEquals(lex, "jz"))
			return TokenInst::JZ;
		else if (Util::strEquals(lex, "jnz"))
			return TokenInst::JNZ;
		else if (Util::strEquals(lex, "and"))
			return TokenInst::AND;
		else if (Util::strEquals(lex, "or"))
			return TokenInst::OR;
		else if (Util::strEquals(lex, "xor"))
			return TokenInst::XOR;
		else if (Util::strEquals(lex, "stb"))
			return TokenInst::STB;

		else if (Util::strEquals(lex, "hlt"))
			return TokenInst::HLT;

		else {
			return -1;
		}
	}

} // namespace VM