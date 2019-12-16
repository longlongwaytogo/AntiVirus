
#include<string>
#include <vector>
#include <fstream>

class Task
{
public:
    Task();
   
    static bool IsDir(std::string& path);
    static void GetFiles(std::string& path,std::vector<std::string>& files);
    static void GetAllPathFile(std::vector<std::string>& files,std::string& path, std::string includeExt = "*.*", std::string excludeExt = "");
   
    bool Excute(std::string& path);
private:
    bool RepairFile(std::ifstream& ifs);

    //// ÊÇ·ñ±»¸ÐÈ¾
    bool IsInfection(std::string& strBuf);

    inline int GetFileSize(std::string& fileName);

    std::string start_line;
    std::string end_line;
    std::vector<std::string> reverse_line_data;
};