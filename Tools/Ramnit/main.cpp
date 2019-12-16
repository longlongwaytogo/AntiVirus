
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <corecrt.h>
#include "Task.h"
void main(int argc, char* argv[])
{
    std::cout << "start Ramnit tools" << std::endl;
    {
        Task task;
        std::string fileOrPath = "I:\\co\\longlongwaytogo.github.io\\_includes\\comments.html";
        if (argc > 1)
        {
            fileOrPath = argv[1];
            std::cout << "Check:" << fileOrPath << std::endl;
        }
        while(fileOrPath.empty() || (_access(fileOrPath.c_str(), 0) == -1))
        {
            std::cout << "path is invalid!" << std::endl;
            std::cout << "please input path or file agin!" << std::endl;
            std::cin >> fileOrPath;
        }

        // do 
        if (Task::IsDir(fileOrPath))
        {
            // find all file 
        }
        else
        {
            task.Excute(fileOrPath);
        }
    }
    std::cout << "press any key to quit!" << std::endl;
    getch();
    std::cout << "end Ramnit tools" << std::endl;

}