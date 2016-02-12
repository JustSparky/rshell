#include <string>
#include <boost/tokenizer.hpp>
class rshell{
    protected:
        string commmands;
        vector<string*> commandlist;
	bool prevCommandPass;
    public:
        rshell(){
	    commands = NULL;
        }
	void parseAllCommands(string com){
            char_separator<char> delims(" ");
	    tokenizer(char_separator<char>> tokenlist(com, delims);
            for (tokenizer<char_separator<char>>::iterator i = tokenlist.begin(); i != tokenlist.end(); i++){
	        std:string com(*i);
		commandlist.push_back(com);
            }
        }
        void createParsed(){
            for(int i = 0; i < commandlist.size(); i++){
	        if(commandlist.at(i) != ";"){
                    
        int executeCommand(){
            char* array = new char[
	    execvp(array0,array)
	    p
        }
	bool checkBreaker(string str){
	    if (str == "||" || str == "&&" || str == ";"){
	        return true;
            }
	}
	bool checkCommandRun(string str){
	    if (str == "||"){
		if(prevCommandPass){
	            return false;
		}
		return true;
	    }
	    if (str == "&&"){
		if(prevCommandPass){
		    return true;
		}
		return false;
	    }
	    if (str == ";"){
		return true;
	    }
	}
