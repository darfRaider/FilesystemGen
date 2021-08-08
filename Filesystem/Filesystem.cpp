#include <filesystem>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
/*
class Logger {
private:
    std::ofstream file;
    char** filename;
    int timestamp;
    void write_header();
    void write_footer();
public:
    Logger(const char*);
    ~Logger();
    void write_log_entry(const char*);
};

*/
struct File {
    uint32_t id;
    std::string filename;
    std::string suffix;
    std::string defaultSuffix;
    std::string path;
    std::string filetype;
    long long modified;
    uint64_t size;
};

class Logger {
private:
    std::ofstream* file;
    const char* filename;
    int timestamp;
    void write_header();
    void write_footer();
public:
    Logger(const char* filename) {
        this->filename = filename;
        this->file = new std::ofstream(filename);
    }
    ~Logger() {
        this->file->close();
        delete this->file;
    };
    void write_log_entry(const char* value) {
        *(this->file) << "LOL" << value;
    }
};


template <typename TP>
std::time_t to_time_t(TP tp);

void print_file(const File& f, std::ofstream& filestream);

int main(int argc, char** argv)
{
    std::cout << "ARGV " << argv[1] << std::endl;
    std::ofstream myFile("test.json");
    std::vector<File> res;
    std::filesystem::path p(argv[1]);
    Logger l("test.log");
    l.write_log_entry("\nTESTLI");

    for (const auto& dirEntry : recursive_directory_iterator(p)){
        try {
            if (dirEntry.is_regular_file()) {
                    File f;
                    f.filename = dirEntry.path().filename().u8string().c_str();
                    f.suffix = dirEntry.path().extension().u8string();
                    f.suffix.erase(f.suffix.begin());
                    f.path = dirEntry.path().u8string().c_str();
                    std::replace(f.path.begin(), f.path.end(), '\\', '/');
                    f.modified = to_time_t(dirEntry.last_write_time());
                    f.size = dirEntry.file_size();
                print_file(f, myFile);
                res.push_back(f);
            }
        }
        catch (const std::exception& e) {
            std::cout << "Exception caught: " << e.what() << std::endl;
        }
    }
}

template <typename TP>
std::time_t to_time_t(TP tp)
{
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
        + system_clock::now());
    return system_clock::to_time_t(sctp);
}

void print_file(const File& f, std::ofstream& filestream) {
    filestream << "{" << std::endl;
    filestream << "\t" << "\"fname\": \"" << f.filename << "\"," << std::endl;
    filestream << "\t" << "\"suffix\": \"" << f.suffix << "\"," << std::endl;
    filestream << "\t" << "\"path\": \"" << f.path << "\"," << std::endl;
    filestream << "\t" << "\"modified\": " << f.modified << "," << std::endl;
    filestream << "\t" << "\"size\": " << f.size << std::endl;
    filestream << "}," << std::endl;
}

