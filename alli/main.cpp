//
//  main.cpp
//  alli
//
//  Created by Gideon Williams on 7/27/15.
//  Copyright (c) 2015 Gideon Williams. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>

using namespace std;

struct command
{
    string alias;
    string command;
};

int main(int argc, const char * argv[])
{
    string commandToRun = "";
    
    if(argc == 1)
    {
        cout    << "Commands: [v0.1.0]" << endl
                << " add <alias> < command > -- add or replace a command" << endl
                << " remove <alias>          -- remove an existing command" << endl
                << " list                    -- list exisiting commands" << endl
                << " <alias> <args>          -- run a command" << endl;
    } else
    {
        if(string(argv[1]) == "list")
        {
            commandToRun = "list";
        }else if(string(argv[1]) == "add" && argc >= 3)
        {
            commandToRun = "add";
        }else
        {
            // run command
            commandToRun = string(argv[1]);
        }
    }

    const char *homeDir = getenv("HOME");
    if (!homeDir)
        cerr << "Unable to find home directory." << endl;

    
    //save it in a .file for every user
    string path(homeDir);
    string file = "/.alli";
    fstream fs;
    vector<shared_ptr<command>> commands;
    
    fs.open(path+file);
	
	//create file if it doesn't exist
	if(!fs.is_open())
	{
		fs.clear();
		fs.open(path+file, ios_base::out);
		fs.close();
		fs.open(path+file);
	}
	
    if(fs.is_open())
    {
        if (commandToRun == "add") {
            //write command to file
            stringstream ss;
            string aliasToAdd = "";
            string commandToAdd = "";
            
            ss << argv[2];
            aliasToAdd = ss.str();
            ss.str("");
            
            for(int i = 3; i < argc; i++)
            {
                ss << argv[i] << " ";
            }
            commandToAdd = ss.str();
            
            fs.seekg(0, ios_base::end);
            fs << aliasToAdd << endl;
            fs << commandToAdd << endl;
            fs << "--" << endl;
            fs.flush();
            fs.close();
            
        } else {
            int count = 0;
            shared_ptr<command> tempCommand;
            for(array<char, 1024> line; fs.getline(&line[0], 1024);)
            {
                string lineCheck(line.data());
                if (count == 0) {
                    tempCommand = make_shared<command>();
                    tempCommand->alias = lineCheck;
                    count++;
                }else if (count == 1) {
                    tempCommand->command = lineCheck;
                    commands.push_back(tempCommand);
                    count++;
                } else {
                    count = 0;
                }
            }
            for (auto cmd : commands) {
                if (commandToRun == "list") {
                    cout << cmd->alias << " -> " << cmd->command << endl;
                }
                if (commandToRun == cmd->alias) {
					stringstream ss;
					ss << cmd->command.c_str();
					for(int i = 2; i < argc; i++)
					{
						ss << argv[i] << " ";
					}
                    system(ss.str().c_str());
                }
            }
        }
    } else {
        cerr << "Unable to open file store: " << path << file << endl;
    }
    
    
    return 0;
}
