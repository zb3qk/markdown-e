#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <map> 

using namespace std;

string DICT_NOT_PARAM = "The dictionary file could not be found in the parameters";
string DICT_NOT_FOUND = "The specified dictionary could not be found in the filesystem";

struct head
{
    string statement;
    string location;
};

struct func{
    string name;
    vector<string> parameters;
    string content;
};

struct
{
    vector<struct head> header;
    map<string, struct func> entries;
} mded; // struct to be saved to a file


void exit_protocol(string error){
    cout << "Error: " << error << endl;
    exit(1);
}

void exit_protocol(string error, int line_number, string line){
    cout << "Error: " << error << endl;
    cout << "Line " << line_number << ": " << line << endl;
    exit(1);
}

void directory_compile(vector<string> tokens){
    string dictionary = tokens[0];
    vector<string> elements;

    // split string to check if file is a '.mded'
    std::stringstream ss(dictionary);
    char delim = '.';
    std::string token;
    while (std::getline(ss, token, delim)) {
        elements.push_back(token);
        cout << token << endl;
    }


    if ((elements[elements.size()-1].compare("mded")) )
        exit_protocol(DICT_NOT_PARAM + "\ninput: " + dictionary + "\n");
    
    ifstream dictionary_file;
    dictionary_file.open (dictionary);

    if (dictionary_file.fail())
         exit_protocol(DICT_NOT_FOUND + "\ninput: " + dictionary + "\n");    

    
    // Parse the dictionary file
    map<string, string *> entries; 

    int line_number = 1;

    /* Head of .mded file */
    vector<struct head> header;

    /* Parse each line of the file */
    string line;
    string content;
    struct func cur_func;
    cur_func.content = "";

    while (!dictionary_file.eof())
    {   
        getline(dictionary_file,line);
        if (line.empty()){
            continue;
        }
        istringstream iss(line);
        std::vector<std::string> tokens((std::istream_iterator<std::string>(iss)),
                                 std::istream_iterator<std::string>());
        /* -- import: styles.css */
        if (tokens[0].compare("--") == 0){
            cout << "import: " << line << endl;
            struct head cur_head;
            cur_head.statement = tokens[1]; cur_head.location = tokens[2]; 
            header.push_back(cur_head);
        }
        /* func: (param_1, param_2, param_3) */
        else if (tokens[0].back() == ':')
        {
        cout << "function: " << line << endl;
        if (!cur_func.content.compare("")){
            cur_func.content = content;
            mded.entries.insert(std::pair(cur_func.name,cur_func));
        }
        struct func fun; vector<string> parameters;
        cur_func = fun;
        tokens[0].pop_back();
        fun.name = tokens[0]; fun.parameters = parameters;

        // Invalid parentheses
        if (tokens[1].front() != '(')
            exit_protocol("Could not find opening parentheses on first character after function declaration", line_number, line);
        if (tokens[tokens.size()-1].back() != ')')
            exit_protocol("Could not find closing parentheses on final character of line", line_number, line);
        
        tokens[tokens.size()-1].pop_back(); // remove closing paren
        tokens[1].erase(0,1); // remove opening paren

        vector<string> params;
        for (int i = 1; i < tokens.size(); i++){
            istringstream el(tokens[i]);
            std::string param;
            while(std::getline(el, param, ',')) {
                params.push_back(param);
                std::cout << "param: " << param << '\n';
            }
        }

        }
        /* parse content of function */
        else {
            
            content += line+"\n";
        }

    line_number += 1;
    }       
}

void file_compile(vector<string> tokens){

}

int main( int argc, const char* argv[] )
{
    argc += 1;
    argv[1] = "example.mded";
    /*
        Possible user input:
            # Compiles one file #
            file.mde
            source.mded
        or
            # Compiles all files in the directory #
            source.mded 
    */
    // Take user input
	vector <string> tokens;
    for (int i = 1; i < argc; ++i) 
        tokens.push_back(argv[i]);

    if (tokens.size() == 1)
        directory_compile(tokens);
    else if (tokens.size() == 2)
        file_compile(tokens);
    else
    {
      cout << "Please " << endl; 
    }
    
}