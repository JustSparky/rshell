#include <string>
#include <boost/tokenizer.hpp>
class rshell{
    protected:
        string commmands;
        vector<string> commandlist;
    public:
        rshell(){
		commands = NULL;
        }
	void parseAllCommands(string com){
            char_separator<char> delims(" ");
	    tokenizer(char_separator<char>> tokenlist(com, delims);
            for (tokenizer<char_separator<char>>::iterator i = tokenlist.begin(); i != tokenlist.end(); i++){
	        std::string command(*i);
		commandlist.push_back(command);
            }
       }
       int executeCommand(){
            char* array = new char[
	    execvp(array0,array)
	    p
        }

