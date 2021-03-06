#ifndef SPACE_PARSER_H
#define SPACE_PARSER_H

#include <VM/Util.h>
#include <VM/Token.h>
#include <VM/Compiler.h>

#include <cstdlib>

namespace VM {

    class Parser {
    private:
        char* text;
		bool debug = false;
		bool symbols = false;
		int flen;
		
		bool nextPPI = false;
		int lastStoreSize = 0;
		
		TokenList* tokenList;
		List<char*>* lblList;
		List<char*>* jmpList;
		List<char*>* varList;
		List<char*>* addrList;
		List<char*>* strList;
		
		const char* beginLabel = "start";

    public:
        Parser(char* text, int flen);
        ~Parser();

		void showTokenList();
		void showLabelLists();
		Compiler* createCompiler(const char* path);
		void ppi(char* inst, int line);
		void includeFile(char* file);

        void setDebug(bool debug) { this->debug = debug; }
        void setSymbols(bool symbols) { this->symbols = symbols; }
        inline bool isDebug() const { return debug; }
		inline TokenList* getTokenList() const { return tokenList; }

        void start();
		int checkInst(char* lex);
    };

} // namespace VM

#endif // SPACE_PARSER_H