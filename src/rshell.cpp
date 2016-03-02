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
		bool forceExit;
	public:
		//Constructor
        	rshell(){
	    		commands = "";
	    		nextConnector = ";";
	    		prevCommandPass = true;
	    		forceExit = false;
		}
		// Parses the string (strings ending in ';' will keep the ';')
		void parseAllCommands(){
			char_separator<char> delims(" ");
    			tokenizer<char_separator<char> > tokenlist(commands, delims);
		       	for (tokenizer<char_separator<char> >::iterator i = tokenlist.begin(); i != tokenlist.end(); i++){
		    		string com(*i);
				commandlist.push_back(com);
        		}
    		}

		// Executes one command and sets prevCommandPass to true or false.
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
						//Exit check
       	        				if (commandlist.at(i) == "exit"){
							cout << "Forced Exit.";
                   					forceExit = true;
	        					return;
	        				}
						// Comment check
            					if (commandlist.at(i) == "#" || checkComment(commandlist.at(i))){
		        				executeCommand(commandsublist);
		        				return;
		        			}
						//Adds command to the list
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

		//	Checks if there is a '#' at the front of the string
		bool checkComment(string str){
			if (str.at(0) == '#'){
				return true;
			}
			return false;
		}

		//	Checks if the string is a breaker
		bool checkBreaker(string str){
			if (str == "||" || str == "&&" || str == ";"){
		        	return true;
			}
			return false;
		}

		// Checks if the next command should be run
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
