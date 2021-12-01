// #include "Files.hpp"

// std::mutex mute_directory;
// std::unique_ptr<std::FILE,std::function<void(std::FILE *)>> Files::file(NULL,[](std::FILE * __stream)->void{
//     std::fclose(__stream);
// });

// std::vector<std::string> Files::static_filepath=[]()->std::vector<std::string>{
//     std::string staticdir=SETTINGS::STATIC_DIR;
//     std::vector<std::string> temporary;
//     for(const auto& entry : fs::recursive_directory_iterator(staticdir.c_str())){
//         temporary.emplace_back(entry.path().relative_path());
//     }
//     return temporary;
// }();

// std::vector<std::string> Files::templates_filepath=[]()->std::vector<std::string>{
//     std::vector<std::string> temporary;
//     std::string templatesdir;
//     for(auto I: SETTINGS::TEMPLATES_DIR){
//         templatesdir=I;
//         for(const auto& entry : fs::recursive_directory_iterator(templatesdir.c_str())){
//             temporary.emplace_back(entry.path().relative_path());
//         }
//     }
//     return temporary;
// }();

// //get filepath of .html files
// std::string Files::get_files_path(const std::string& filesname,enum Filetype type){
    
//         switch (type){
//             case Filetype::template_files:
//                 {
//                     std::lock_guard<std::mutex> lock(mute_directory);
//                     for(auto& file:templates_filepath)
//                     {
//                         if(file.find(filesname)!=std::string::npos)
//                             return file;
//                     }
//                     break;
//                 }
//             case Filetype::static_files:
//                 {
//                     std::lock_guard<std::mutex> lock(mute_directory);
//                     for(auto& file:static_filepath)
//                     {
//                         if(file.find(filesname)!=std::string::npos){
//                             return file;
//                         }
//                     }
//                     break;
//                 }
//             default:
//                 return "";
//     }
//     return "";
// }

// //Read data of file to string buffer
// void Files::readFileToBuffer(std::string& filename,enum Filetype type,int internal){
//     unsigned int fsize = 0;
//     const std::string filepath = internal == 0 ? filename : Files::get_files_path(filename, type);
//     if(!filepath.empty())
//     {
//         std::lock_guard<std::mutex> lock(mute_directory);
//         Files::file.reset(fopen(filepath.c_str(),"r"));
//         if(Files::file)
//         {
//             std::fseek(Files::file.get(),0L,SEEK_END);
//             fsize = std::ftell(Files::file.get());
//             std::rewind(Files::file.get());
//             std::string temp(fsize, '\0');
//             if(fsize != fread(&temp[0],1,fsize,Files::file.get()))
//                 std::cerr <<"File data Fetch failed"<< filepath<<'\n';
//             data = std::move(temp);
//         }
//         else
//         {
//             std::cerr << "File Not found: "<<filename<<'\n';
//         }
//     }
// }

// void Files::getBufferData(std::string& gDat){
//     if(!data.empty())
//         gDat = data;
// }
