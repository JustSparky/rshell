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
	//Variables
        string commands;
	string nextConnector;
        vector<string> commandlist;
	bool prevCommandPass;
	bool forceExit;
    public:
	//Constructor
        rshell(){
	    commands = "";
	    nextConnector = ";";
	    prevCommandPass = true;
	    forceExit = false;
        }
	//Separates string into tokens and puts them into commandlist, if the token ends with a ;, it stays with the string
	void parseAllCommands(){
            char_separator<char> delims(" ");
	    tokenizer<char_separator<char> > tokenlist(commands, delims);
            for (tokenizer<char_separator<char> >::iterator i = tokenlist.begin(); i != tokenlist.end(); i++){
	        string com(*i);
		commandlist.push_back(com);
            }
        }
	//Executes one command and sets prevCommandPass to true or false.
	void executeCommand(vector<string> com){
	    char* argv[1024];
	    int pid_child;
	    int status_child;
             
            for(unsigned int i = 0; i < com.size(); i++){
	        argv[i] = (char*)com.at(i).c_str();
            }
	    argv[com.size()] = NULL;
	    pid_child = fork();
	    if (pid_child == 0){
	        if(execvp(argv[0], argv) == -1){
		    perror("execvp failed: ");
		    prevCommandPass = false;
		    return;
		}
	    }
            else{
	        if (waitpid(pid_child, &status_child, 0) == -1){
		    perror("pid incorrect: ");
		}
	    }
	    prevCommandPass = true;
	}
	//Splits commandlist into commands with their arguments then calls executeCommand to run them.
	void executeAllCommands(){
            vector<string> commandsublist;
	    unsigned int i = 0;
	    unsigned int j = 0;
	    while (i < commandlist.size()){
		j = 0;
		if (checkCommandRun()){
	            while (!checkBreaker(commandlist.at(i))){
	       	        if (commandlist.at(i) == "exit"){
	                    forceExit = true;
		            exit(1);
		        }
	                if (commandlist.at(i) == "#"){
		            executeCommand(commandsublist);
		            return;
		        }
	                commandsublist.push_back(commandlist.at(i));
			if (commandsublist.at(j).at(commandsublist.at(j).length() - 1) == ';'){
			    commandsublist.at(j).erase(commandsublist.at(j).length() - 1);
			    nextConnector = ';';
			    break;
			}
		        i++;
			j++;
			if (i == commandlist.size()){
			    executeCommand(commandsublist);
			    return;
			}
                    }
                    if (checkBreaker(commandlist.at(i))){
    		        nextConnector = commandlist.at(i);
		    }
		    i++;
		}
		executeCommand(commandsublist);
		commandsublist.clear();
	    }
	}
	//Checks if the string is a breaker
	bool checkBreaker(string str){
	    if (str == "||" || str == "&&" || str == ";"){
	        return true;
            }
	    return false;
	}
	//Checks if the next command should be ran.
	bool checkCommandRun(){
	    if (nextConnector == "||"){
		if(prevCommandPass){
	            return false;
		}
		return true;
	    }
	    if (nextConnector == "&&"){
		if(prevCommandPass){
		    return true;
		}
		return false;
	    }
	    if (nextConnector == ";"){
		return true;
	    }
	    return true;
	}
	//Starts the program.
	void run(){
	    while (!forceExit){
		cout << "$";
	        getline(cin, commands);
		if (commands != "" && commands != "exit"){
		    parseAllCommands();
                    executeAllCommands();
		}
		if(commands == "exit"){
		    forceExit = true;
		}
		if(forceExit){
		    cout << "Forced Exit." << endl;
		    return;
		}
		commandlist.clear();
	    }
	}
};
