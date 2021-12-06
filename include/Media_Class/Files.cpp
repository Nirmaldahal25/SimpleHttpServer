#include "Files.hpp"
#include <iostream>
const std::string path;
const std::filesystem::path HOSTED_FOLDER = []()->auto{
    std::filesystem::current_path(path);
    return std::filesystem::current_path();
}();

void Directory::setData(std::string relative_path, enum Type typeofFile)
{
    if(relative_path.find_first_of('/') == 1)
    {
        relative_path.erase(relative_path.begin(),relative_path.begin()+2);
    }
    
    this->file_folder.emplace_back(std::move(relative_path));
    this->types.push_back(typeofFile);
}

// std::filesystem::current_path(std::filesystem::current_path());
Directory Files::getDirectory(const std::string& relative_path)
{
    Directory directory;
    
    for(const auto& dir_entry : std::filesystem::directory_iterator(relative_path))
    {
        if(dir_entry.is_directory())
        {
            directory.setData(dir_entry.path(), Directory::Type::Folder);
        }
        else if(dir_entry.is_regular_file())
        {
            directory.setData(dir_entry.path(),Directory::Type::File);
        }
    }
    return directory;
}

std::string Files::getDirectoryView(const std::string& relative_path)
{   
    const auto directory = getDirectory(relative_path);
    std::string directoryview("<ul>");

    for(int i=0; i<directory.size(); i++)
    {
        directoryview += std::string{"<li><a href="} + '\"' + directory.file_folder[i] + "\">" + directory.file_folder[i];
        switch(directory.types[i])
        {
            case Directory::Type::Folder:
                directoryview += "</a>&nbsp&nbsp Folder<br>";
                break;
            case Directory::Type::File:
                directoryview +=  "</a>&nbsp&nbsp File<br>";
                break;
            default:
                return directoryview + "</ul>";    
        }
    }
    return directoryview + "</ul><br>"; 
}

std::tuple<bool,enum Directory::Type> Files::checkForFileorFolder(const std::string& relative_path)
{
    bool exists = std::filesystem::exists(relative_path);

    if(exists)
    {
        const std::filesystem::path path(relative_path);
        if(std::filesystem::is_directory(path)) return std::make_tuple<bool,enum Directory::Type>(true, Directory::Type::Folder);
        if(std::filesystem::is_regular_file(path)); return std::make_tuple<bool,enum Directory::Type>(true, Directory::Type::File);
    }
    return std::make_tuple<bool,enum Directory::Type>(false, Directory::Type::NoneType);
}

bool Files::readFromFile(const std::string& filename, std::string& buff, int offset)
{
    std::unique_ptr<int,void(*)(int*)> fd(NULL,[](int* fd)->void{ close(*fd); });
    fd.reset(new int(open(filename.c_str(), O_RDONLY)));
    int rd = 0, total_read = 0;

    if(*fd = -1) return false;
    
    total_read = lseek(*fd, 0, SEEK_END);
    lseek(*fd,offset,SEEK_SET);

    std::vector<char> data(total_read);

    while (offset != total_read)
    {
        rd = pread(*fd, &data[offset], total_read-offset, offset);
        if(rd == -1)
        {
            buff.clear();
            buff.reserve(offset);
            buff.insert(buff.begin(),data.begin(),data.begin()+offset);
            return false;
        }
        offset += rd;
    }
    buff.clear();
    buff.reserve(offset);
    buff.insert(buff.begin(),data.begin(),data.begin()+offset);
    return true;
}

bool Files::writeToFile(const std::string& filename, std::string& buff, int offset)
{
    std::unique_ptr<int,void(*)(int*)> fd(NULL,[](int* fd)->void{ close(*fd); });
    fd.reset(new int(open(filename.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR)));
    int rd = 0, filesize = 0;

    if(*fd = -1) return false;

    filesize = lseek(*fd, 0, SEEK_END);
    lseek(*fd,filesize > 0 ? filesize : offset,SEEK_SET);

    const char* buf = buff.c_str();

    while (offset != buff.size())
    {
        rd = pwrite(*fd, &buf[offset], buff.size() - offset, filesize + offset);
        if(rd == -1)
        {
            buff.erase(buff.begin() + offset);
            return false;
        }
        offset += rd;
    }
    buff.clear();
    return true;
}