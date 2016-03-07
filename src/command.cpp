#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <sys/wait.h>
#include <sys/stat.h>

using namespace std;

class command{
	protected:
		vector<string> commandlist;
		bool commandPass;
		string commandType;
		bool allCount;
		string nextConnector;
	public:
		command(){}
		command(vector<string> c){
			commandlist = c;
			commandType = ";";
			allCount = true;
			nextConnector = ";"
		}
		command(vector<string> c, string t){
			commandlist = c;
			commandType = t;
			allCount = true;
			nextConnector = ";"
		}
		bool getPass(){
			return commandPass;
		}
		string getType(){
			return commandType;
		}
		void runCommand(vector<string> com){
			char* argv[1024];

			for(unsigned int i = 0; i < com.size(); i++){
				argv[i] = (char*)com.at(i).c_str();
			}
			argv[com.size()] = NULL;

			pid_t pid;
			int status;
			pid = fork();
			if (pid == 0){
				prevCommandPass = true;
				execvp(argv[0], argv);
				perror("execvp failed: ");
				exit(-1);
			}
			else{
				if (waitpid(pid, &status, 0) == -1){
					perror("Wait: ");
				}
				if (WIFEXITED(status) && WEXITSTATUS(status) != 0){
					prevCommandPass = false;
				}
			}
		}
		void runAllCommands(){
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
							runCommand(commandsublist);
							return;
						}
						if (commandlist.at(i) == "["){
							i++;
							commandsublist.push_back(commandlist.at(i);
							if (commandlist.at(i) == "-e" || commandlist.at(i) == "-f" || commandlist.at(i) == "d"){
								i++;
								commandsublist.push_back(commandlist.at(i));
							}
							else{
								i++;
							}
							if (commandlist.at(i) == "]"){
								i++;
								checkTest(commandsublist);
								commandsublist.clear();
							}
							else{
								cout << "Error: Missing close bracket." << endl;
								_exit(1);
							}
							break;
						}
						if (commandlist.at(i) == "test"){
							i++;
							commandsublist.push_back(commandlist.at(i));
							if (commandlist.at(i) == "-e" || commandlist.at(i) == "-f" || commandlist.at(i) == "d"){
								i++;
								commandsublist.push_back(commandlist.at(i));
							}
							else{
								i++;
							}
							checkTest(commandsublist);
							commandsublist.clear();
							break;
						}
								
						//Adds command to the list
						commandsublist.push_back(commandlist.at(i));
						i++;
						j++;
						if (i == commandlist.size()){
							runCommand(commandsublist);
							return;
						}
					}
					if (commandsublist.size() > 0){
						runCommand(commandsublist);
						commandsublist.clear();
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
				return false;
			}
			else if (nextConnector == ";"){
				return true;
			}
			return false;
		}
		
		bool checkTest(vector<string> temp){
			if (temp.at(0) == "-e"){
				return fileExists(temp.at(1));
			}
			else if (temp.at(0) == "-f"){
				return regFileExists(temp.at(1));
			}
			else if (temp.at(0) == "-d"){
				return dirExists(temp.at(1));
			}
			else{
				return fileExists(temp.at(1));
			}
		}
		bool fileExists(string& path){
			struct stat buffer;
			return (stat(file, &buffer) == 0);
		}

		bool dirExists(string& path){
			struct stat buffer;
			if (stat(path, &buffer) == 0 && S_ISDIR(buffer.st_mode)){
				return true;
			}
			return false;
		}
		
		bool regFileExists(string& path){
			struct stat buffer;
			if (stat(path, &buffer) == 0 && S_ISREG(buffer.st_mode)){
				return true;
			}
			return false;
		}
		void execute(bool prevCommand){
			if (prevCommand){
				if (commandType == "&&"){
					runAllCommands();
					if (allCount){
						commandPass = true;
					}
					else{
						commandPass = false;
					}
				}
				else if (commandType == "||"){
					commandPass = true;
				}
				else if (commandType == ";"){
					runAllCommands();
					commandPass = true;
			}
			else{
				if (commandType == "&&"){
					commandPass = false;
				}
				else if (commandType == "||"){
					runAllCommands();
					if (allCount){
						commandPass = true;
					}
					else{
						commandPass = false;
					}
				}
				else if (commandType == ";"){
					runAllCommands();
					commandPass = true;
				}
			}
		}

};
