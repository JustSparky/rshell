#include <string>
#include <vector>
#include <boost/tokenizer.hpp>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <sys/wait.h>
#include <queue>

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
					


			

		void queueBuilder(){
			for(int i = 0; i < commandlist.size(); i++){
				
				

		//Splits commandlist into commands with their arguments then calls executeCommand to run them.
		void executeAllCommands(){
			vector<string> commandsublist;
			unsigned int i = 0;
			unsigned int j = 0;
			while (i < commandlist.size()){
				j = 0;
				if (checkCommandRun()){
					while (!checkBreaker(i)){
						//Exit check
						if (commandlist.at(i) == "exit"){
							cout << "Forced Exit." << endl;
							forceExit = true;
							_Exit(0);
						}
						// Comment check
						if (commandlist.at(i) == "#" || checkComment(commandlist.at(i))){
							executeCommand(commandsublist);
							return;
						}

						//Adds command to the list
						commandsublist.push_back(commandlist.at(i));
						i++;
						j++;
						if (i == commandlist.size()){
							executeCommand(commandsublist);
							return;
						}
					}
					executeCommand(commandsublist);
					commandsublist.clear();
					if (checkBreaker(i)){
						if (nextConnector == "||"){
							if (allCount == true){
								prevCommandPass = true;
							}
							else{
								if (prevCommandPass == false){
									allCount = false;
								}
								else{
									allCount = true;
								}
							}
						}
						else if (nextConnector == "&&"){
							if (allCount == true){
								if (prevCommandPass == false){
									allCount = false;
								}
							}
							else{
								allCount = false;
								prevCommandPass = false;
							}
						}
						else if (nextConnector == ";"){
							if (prevCommandPass == true){
								allCount = true;
							}
							else{
								allCount = false;
							}
						}
						if (commandlist.at(i) == "|"){
							nextConnector = "||";
						}
						else if (commandlist.at(i) == "&"){
							nextConnector = "&&";
						}
						else if (commandlist.at(i) == ";"){
							nextConnector = ";";
						}
						i++;
					}
					i++;
				}
				else{
					i++;
				}
			}
		}

		//	Checks if there is a '#' at the front of the string
		bool checkComment(string str){
			if (str.at(0) == '#'){
				return true;
			}
			return false;
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
	

		// Checks if the next command should be run
		bool checkCommandRun(){
			if (nextConnector == "||"){
				if(allCount == true){
					return false;
				}
				else{
					return true;
				}
			}
			else if (nextConnector == "&&"){
				if(allCount == true){
					return true;
				}
				else{
					return false;
				}
			}
			else if (nextConnector == ";"){
				return true;
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
