#include <string>
#include <vector>
#include <boost/tokenizer.hpp>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <sys/wait.h>

using namespace boost;
using namespace std;

class rshell{
	protected:
		//Data Members
		string commands;
		string nextConnector;
		vector<string> commandlist;
		bool prevCommandPass;
		bool allCount;
		bool forceExit;
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
			char_separator<char> delims(" ","&,;,|");
			/* code for parenthesis/brackets
			 * char_separator<char> delims(" ", " &,;,|,(,),[,]");
			*/
			tokenizer<char_separator<char> > tokenlist(commands, delims);
			for (tokenizer<char_separator<char> >::iterator i = tokenlist.begin(); i != tokenlist.end(); i++){
				string com(*i);
				commandlist.push_back(com);
			}
		}

		// Executes one command and sets prevCommandPass to true or false.
		void executeCommand(vector<string> com){
			char* argv[1024];
			pid_t p = getpid();
			pid_t pid = fork();
			int status;

			for(unsigned int i = 0; i < com.size(); i++){
				argv[i] = (char*)com.at(i).c_str();
			}
			argv[com.size()] = NULL;

			if (pid == 0){
				prevCommandPass = true;
				execvp(argv[0], argv);
				perror("execvp failed: ");
				prevCommandPass = false;
				_exit(1);
			}
			else if (pid > 0){
				if ((p = wait(&status)) < 0){
					perror("child failed: ");
					prevCommandPass = false;
					_exit(1);
				}
			}
			else{
				perror("fork failed: ");
				_exit(1);
			}
		}	

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
							allCount = true;
							prevCommandPass = true;
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
					}
					i++;
				}
				executeCommand(commandsublist);
				commandsublist.clear();
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
			if ( (unsigned)i < commandlist.size() - 1){
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
			else if( (unsigned)i == commandlist.size() - 1){
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
