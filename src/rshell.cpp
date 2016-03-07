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
		queue<command> vecCommands;
		bool prevCommandPass;
		bool allCount;
		bool forceExit;
		int prevParen;
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
		}
		
		// Builds commands and places them in a queue in order of precedence
		void commandBuilder(){
			vector<string> tempVec;
			string tempConnect;
			for(int i = 0; i < commandlist.size(); ++i){
				if(!checkParen(i)){ 
					if(i != 0){
						if( (commandlist.at(i - 1) == ")") && (checkbreaker(i)) ){
							if( commandlist.at(i) == ";"){
								tempConnect = ";";
							}
							else if( commandlist.at(i) == "|"){
								tempConnect = "||";
								i++;
							}
							else{
								tempConnect = "&&";
								i++;
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
						vecCommands.push( command(tempVec) );
					}
					else{
						vecCommands.push( command(tempVec, tempConnect) );
						tempConnector.clear();
					}
				}
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
		bool checkBrack(unsigned i_){
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
				executeAllCommands();
				commandlist.clear();
				nextConnector = ";";
				prevCommandPass = true;	
			}
		}
};
