#ifndef GETFILES_H
#define GETFILES_H

#include <string>
#include <iostream>
#include <vector>
void list_folders(std::string path, std::vector<std::string> &folders);
void list_file(std::string path, std::vector<std::string> &files);

#endif // GETFILES_H
