#ifndef FILES 
#define FILES 1

#include <string>
#include <unistd.h>
#include <filesystem>
#include <vector>
#include <fcntl.h>
#include <utility>
#include <tuple>

struct Directory
{
    enum class Type{
        Folder, 
        File,
        NoneType
    };
    void setData(std::string relative_path, enum Type typeofFile);
    int size()const
    {
        return this->file_folder.size();
    }
    std::vector<std::string> file_folder;
    std::vector<Directory::Type> types;
};

namespace Files
{
    // return true if a file exists in the path
    std::tuple<bool, enum Directory::Type> checkForFileorFolder(const std::string& relative_path);

    // Return true if all characters are written to file, written data is erased from buff
    bool writeToFile(const std::string& filename, std::string& buff, int offset = 0);

    // Return true if all characters are obtained, buff.size() is the size obtained
    bool readFromFile(const std::string& filename, std::string& buff, int offset = 0);

    Directory getDirectory(const std::string& relative_path);

    std::string getDirectoryView(const std::string& relative_path);
}

#endif