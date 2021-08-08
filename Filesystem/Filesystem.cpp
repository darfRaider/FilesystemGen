// Filesystem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <filesystem>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

struct File {
    uint32_t id;
    std::string filename;
    std::string suffix;
    std::string path;
    std::string modified;
    uint64_t size;
};



void print_file(const File& f, std::ofstream & filestream) {
    filestream << "{" << std::endl;
    filestream << "\t" << "\"fname\": \"" << f.filename << "\"," << std::endl;
    filestream << "\t" << "\"suffix\": \"" << f.suffix << "\"," << std::endl;
    filestream << "\t" << "\"path\": \"" << f.path << "\"," << std::endl;
    filestream << "\t" << "\"size\": " << f.size << std::endl;
    filestream << "}," << std::endl;
}

int main(int argc, char** argv)
{
    std::cout << "ARGV " << argv[1] << std::endl;
    std::ofstream myFile("test.json");
    std::vector<File> res;
    std::filesystem::path p(argv[1]);

    for (const auto& dirEntry : recursive_directory_iterator(p)){
        try {
            if (dirEntry.is_regular_file()) {
                File f;
                    //std::filesystem::u8path p(dirEntry.path().filename());

                    f.filename = dirEntry.path().filename().u8string().c_str();
                    f.suffix = dirEntry.path().extension().u8string();
                    f.suffix.erase(f.suffix.begin());
                    f.path = dirEntry.path().u8string().c_str();
                    std::replace(f.path.begin(), f.path.end(), '\\', '/');
                    //f.modified = dirEntry.last_write_time();
                    f.size = dirEntry.file_size();
              
                // catch (std::filesystem::)

                print_file(f, myFile);
                res.push_back(f);
            }
        }
        catch (const std::exception& e) {
            std::cout << "Exception caught: " << e.what() << std::endl;
        }
    }

        /*
            else if (dirEntry.is_directory())
                std::cout << "No FIle: " << dirEntry << std::endl;
            else
                std::cout << "ERROR" << std::endl;*/

    std::cout << "Hello World!\n";
    std::cout << "Len: " << res.size() << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
