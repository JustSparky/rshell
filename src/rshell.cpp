#include <string>
#include <vector>
#include <boost/tokenizer.hpp>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <sys/wait.h>
#include <queue>
#include "command.cpp"

using namespace boost;
using namespace std;

class rshell{
	protected:
		//Data Members
		string commands;
		string nextConnector;
		vector<string> commandlist;
		queue<command*> vecCommands;
		bool prevCommandPass;
		bool allCount;
		bool forceExit;
		int numParen;
		int numBrack;
	public:
		//Constructor
		rshell(){
			commands = "";
			nextConnector = ";";
			prevCommandPass = true;
			forceExit = false;
			allCount = true;
		}

		// Parses the string (strings ending in ';' will keep the ';')
		void parseAllCommands(){
			char_separator<char> delims(" ","&,;,|,(,),[,]");
			tokenizer<char_separator<char> > tokenlist(commands, delims);
			for (tokenizer<char_separator<char> >::iterator i = tokenlist.begin(); i != tokenlist.end(); i++){
				string com(*i);
				commandlist.push_back(com);
			}
			for(unsigned c = 0; c < commandlist.size(); ++c){
				if(commandlist.at(c) == "("){ numParen++; }
				if(commandlist.at(c) == ")"){ numParen--; }
				if(commandlist.at(c) == "["){ numBrack++; }
				if(commandlist.at(c) == "]"){ numBrack--; }
			}
			if(numParen != 0){
				cout << "Invalid number of parenthesis" << endl;
			}
			if(numBrack != 0){
				cout << "Invalid number of brackets" << endl;
			}
		}
		
		// Builds commands and places them in a queue in order of precedence
		void commandBuilder(){
			vector<string> tempVec;
			string tempConnect;
			unsigned int i = 0;
			for(; i < commandlist.size(); ++i){
				if(!checkParen(i)){ 
					if(i != 0){
						if( (commandlist.at(i - 1) == ")") && (checkBreaker(i)) ){
							if( commandlist.at(i) == ";"){
								tempConnect = ";";
								tempVec.push_back(commandlist.at(i));
							}
							else if( commandlist.at(i) == "|"){
								tempConnect = "||";
								tempVec.push_back(commandlist.at(i));
							}
							else{
								tempConnect = "&&";
								tempVec.push_back(commandlist.at(i));
							}
						}
						else{
							tempVec.push_back(commandlist.at(i));
						}
					}
					else{
						tempVec.push_back(commandlist.at(i));
					}
				}
				else if(commandlist.at(i) == ")"){
					if(tempConnect.empty()){
						vecCommands.push(new command(tempVec) );
						tempVec.clear();
					}
					else{
						vecCommands.push(new command(tempVec, tempConnect) );
						tempConnect.clear();
						tempVec.clear();
					}
				}
			}
			if(i == commandlist.size()){
				if (tempVec.size() > 0){
					vecCommands.push(new command(tempVec) );
				}
			}
			else if(commandlist.size() == 1){
				vecCommands.push(new command(tempVec) );
			}
		}

		//Splits commandlist into commands with their arguments then calls executeCommand to run them.
		void executeAllCommands(){
			while (!vecCommands.empty()){
				vecCommands.front()->execute(prevCommandPass);
				prevCommandPass = vecCommands.front()->getPass();
				vecCommands.pop();
			}
		}

		//	Checks if the string is a breaker
		bool checkBreaker(int i){
			if ( (unsigned)i < commandlist.size() + 1){
				if (commandlist.at(i) == "|" && commandlist.at(i + 1) == "|"){
					return true;
				}
				else if (commandlist.at(i) == "&" && commandlist.at(i + 1) == "&"){
					return true;
				}
				else if (commandlist.at(i) == ";"){
					return true;
				}
				else{
					return false;
				}
			}
			else if( (unsigned)i == commandlist.size() + 1){
				if(commandlist.at(i) == ";"){
					return true;
				}
				return false;
			}
			else{
				return false;
			}
		}

		//Checks for Parenthesis
		bool checkParen(unsigned i){
			if( commandlist.at(i) == "(" ){
				return true;
			}
			if( commandlist.at(i) == ")" ){
				return true;
			}
			return false;
		}

		//Checks for brackets
		bool checkBrack(unsigned i){
			if( commandlist.at(i) == "[" ){
				return true;
			}
			if( commandlist.at(i) == "]" ){
				return true;
			}
			return false;
		}

		//Starts the program.
		void run(){
			while (!forceExit && commands != "exit"){
				cout << "$";
				getline(cin, commands);
				parseAllCommands();
				commandBuilder();
				executeAllCommands();
				commandlist.clear();
				nextConnector = ";";
				prevCommandPass = true;	
			}
		}
};
