#include "Task.h"
#include <fstream>
#include <corecrt_io.h>
#include <windows.h>
#include <fileapi.h>
#include <iostream>
#include <vector>
#include <string>

static const int gFilelength = 110 * 1024; // 110kb


int Task::GetFileSize(std::string& fileName)
{
    struct _stat info;
    _stat(fileName.c_str(), &info);
    long size = info.st_size;
    return size;
}


Task::Task()
{
    // find string 
    start_line = "<SCRIPT Language = VBScript> < !--";
    end_line = "//--></SCRIPT>";
    reverse_line_data;
    reverse_line_data.push_back("Set FSO = CreateObject(\"Scripting.FileSystemObject\")");
    reverse_line_data.push_back("DropPath = FSO.GetSpecialFolder(2) & \"\\\" & DropFileName  ");
    reverse_line_data.push_back("If FSO.FileExists(DropPath) = False Then");
    reverse_line_data.push_back("Set FileObj = FSO.CreateTextFile(DropPath, True)");
    reverse_line_data.push_back("For i = 1 To Len(WriteData) Step 2");
    reverse_line_data.push_back("FileObj.Write Chr(CLng(\"&H\" & Mid(WriteData, i, 2)))");
    reverse_line_data.push_back("Next");
    reverse_line_data.push_back("FileObj.Close");
    reverse_line_data.push_back("End If");
    reverse_line_data.push_back("Set WSHshell = CreateObject(\"WScript.Shell\")");
    reverse_line_data.push_back("WSHshell.Run DropPath, 0");
}

bool Task::IsDir(std::string& path)
{
    return GetFileAttributesA(path.c_str())&FILE_ATTRIBUTE_DIRECTORY;
}

void Task::GetFiles(std::string& path, std::vector<std::string>& files)
{
    //文件句柄  
    long   hFile = 0;
    //文件信息  
    struct _finddata_t fileinfo;
    std::string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
    {
        do
        {
            //如果是目录,迭代之  
            //如果不是,加入列表  
            if ((fileinfo.attrib &  _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                     GetFiles(p.assign(path).append("\\").append(fileinfo.name), files);
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
 }

void Task::GetAllPathFile(std::vector<std::string>&files,std::string& path, std::string includeExt /*= "*.*"*/, std::string excludeExt /*= ""*/)
{
    ////获取该路径下的所有文件  
    GetFiles(path, files);
 
#ifdef DEBUG
    int size = files.size();
    for (int i = 0; i < size; i++)                               b
    {
        cout << files[i].c_str() << endl;
    }
#endif 
    
}

bool Task::Excute(std::string& fileName)
{
    std::ifstream ifs;
    ifs.open(fileName);
    if (!ifs.is_open())
        return false;
    ifs.seekg(0, std::ios_base::end);
    int length = ifs.tellg();

    if (length <= gFilelength)
        return false;

    std::cout << "file length:" << std::endl;
  
    //std::string buf;
    const int buf_size = 404+1;
    char buf[buf_size] = {0};
    //buf[buf_size - 1] = '\0';
    //buf[buf_size - 2] = '\0';
    //memset(buf, 0, buf_size);
   
   // int pos = -(buf_size - 1);
    ifs.seekg(-buf_size+1, std::ios_base::end);
    std::ios_base::iostate ist = ifs.rdstate();
    ifs.read(buf, buf_size-1);
    ist = ifs.rdstate();
    ifs.seekg(0, std::ios_base::end);
    ist = ifs.rdstate();
    int cur = ifs.tellg();
    std::string strBuf(buf);
    if (IsInfection(strBuf))  // 已被感染
    {
           // 尝试修复
        RepairFile(ifs);
    }
    ifs.close();
}


bool Task::RepairFile(std::ifstream& ifs)
{
    ifs.seekg(0, std::ios_base::end);
    int length = ifs.tellg();

    if (length <= gFilelength)
        return false;

    std::cout << "file length:" << std::endl;

    //std::string buf;
    
   static char buf[gFilelength];
    memset(buf, 0, gFilelength);


    ifs.seekg(-gFilelength, std::ios_base::end);
    ifs.read(buf, gFilelength);
    std::string strBuf(buf);
  
    return true;
}

bool Task::IsInfection(std::string& strBuf)
{
    bool bFind = false;
    int pos = strBuf.find_last_of(end_line);
    if (pos == std::string::npos)
    {
        bFind = false;
        return false;
    }
    for (int i = reverse_line_data.size() - 1; i >= 0; i--)
        if (strBuf.find_last_of(reverse_line_data[i]) == std::string::npos)
        {
            bFind = false;
            return false;
        }

    bFind = true;
#if DEBUG
    std::cout << strBuf << std::endl;
#endif 
    return bFind;

}

