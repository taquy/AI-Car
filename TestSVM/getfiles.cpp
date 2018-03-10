#include <getfiles.h>
#include <dirent.h>
#include <string.h>
#include <vector>
#include <algorithm>

void list_folders(std::string path, std::vector<std::string> &folders){
      folders.clear();
      struct dirent *entry;
      DIR *dir = opendir(path.c_str());
      while((entry = readdir(dir))!= NULL){
      if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0)) {
       std::string s =  std::string(path) + "/" + std::string(entry->d_name) ;
       folders.push_back(s);
      }
      }
      closedir(dir);

      sort(folders.begin(),folders.end());
}


void list_file(std::string path, std::vector<std::string> &files){

    files.clear();

    struct dirent *entry;

    DIR *dir = opendir(path.c_str());

    while((entry = readdir(dir))!= NULL){
        if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0)) {
             std::string s =  std::string(path) + "/" + std::string(entry->d_name) ;
             files.push_back(s);
        }
    }

    closedir(dir);
    std::sort(files.begin(),files.end());


}
