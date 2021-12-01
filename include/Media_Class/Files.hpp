// #pragma once

// #include <iostream>
// #include <mutex>
// #include <fstream>
// #include <memory>
// #include <functional>
// #include <vector>
// enum class Filetype{
//             static_files,
//             template_files,
//         };

// class Files{
//     public:
//         Files(){

//         }
//         void readFileToBuffer(std::string& filename,enum Filetype type=Filetype::template_files,int internal=0);
//         void getBufferData(std::string&);
//         ~Files(){
//         }
//     private:
//         std::string data;
//         static std::vector<std::string> static_filepath;
//         static std::vector<std::string> templates_filepath;
//         static std::string get_files_path(const std::string& template_name,enum Filetype type=Filetype::template_files);
//         static std::unique_ptr<std::FILE,std::function<void(std::FILE *)>> file;
// };
