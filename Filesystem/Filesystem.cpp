#include <algorithm>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "md5.h"

using recursive_directory_iterator =
    std::filesystem::recursive_directory_iterator;
struct File {
  uint32_t id;
  std::string filename;
  std::string suffix;
  std::string defaultSuffix;
  std::string path;
  std::string filetype;
  std::string md5filehash;
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
  void write_log_entry(const char* value) { *(this->file) << "LOL" << value; }
};

template <typename TP>
std::time_t to_time_t(TP tp);

void print_file(const File& f, std::ofstream& filestream);

bool cmp_strings(const File* a, const File* b) {
  int n_a = a->filename.length();
  int n_b = b->filename.length();
  char x, y;

  for (int i = 0; i < std::min(n_a, n_b); i++) {
    x = std::toupper(a->filename[i]);
    y = std::toupper(b->filename[i]);
    if (x != y) return x < y;
  }
  return n_a < n_b;
  // return std::toupper(x) < std::toupper(y);
  /*  int n_a = a->filename.length();
    std::cout << n_a << std::endl;
    int n_b = b->filename.length();
    std::cout << "b " << n_b << std::endl;
    char x, y;
    std::cout << "exit" << std::endl;
    for (int i = 0; i < std::min(n_a, n_b); i++) {
      x = std::tolower(a->filename[i]);
      y = std::tolower(b->filename[i]);
      if (x < y) return true;
      if (y > x) return false;
    }
    return (n_a > n_b);*/
}

int main(int argc, char** argv) {
  const size_t BufferSize = 144 * 7 * 1024;
  std::istream* input = NULL;
  std::ifstream file;

  int n_reads_failed = 0;
  std::string root_dir_str;
  if (argc > 1) {
    root_dir_str = argv[1];
  } else {
    root_dir_str = ".";
  }
  std::string fname = "test.json";
  std::ofstream myFile(fname.c_str());
  std::vector<File*> res;
  std::string root_dir(root_dir_str);
  std::filesystem::path p(root_dir.c_str());
  Logger l("test.log");
  l.write_log_entry("\nTESTLI");

  for (const auto& dirEntry : recursive_directory_iterator(p)) {
    try {
      if (dirEntry.is_regular_file()) {
        File* f = new File;
        f->filename = dirEntry.path().filename().u8string().c_str();
        std::string suffix = dirEntry.path().extension().u8string();
        if (!suffix.empty()) {
          suffix.erase(suffix.begin());
          f->suffix = suffix;
        }
        f->path = dirEntry.path().u8string().c_str();
        std::replace(f->path.begin(), f->path.end(), '\\', '/');
        f->modified = to_time_t(dirEntry.last_write_time());
        f->size = dirEntry.file_size();
        res.push_back(f);
        // MD5 stuff
        MD5 md5;
        char* buffer = new char[BufferSize];
        file.open(f->path.c_str(), std::ios::in | std::ios::binary);
        input = &file;
        while (*input) {
          input->read(buffer, BufferSize);
          std::size_t numBytesRead = size_t(input->gcount());
          md5.add(buffer, numBytesRead);
        }
        f->md5filehash = md5.getHash().c_str();
        file.close();
        //     print_file(*f, myFile);
        delete[] buffer;
      }
    } catch (const std::exception& e) {
      n_reads_failed++;
      std::cout << "Exception caught: " << e.what() << std::endl;
    }
  }
  std::sort(res.begin(), res.end(), cmp_strings);
  int id_cnt = 0;
  for (auto e : res) {
    e->id = id_cnt;
    print_file(*e, myFile);
    std::cout << e->filename << std::endl;
    id_cnt++;
  }
  myFile.close();
  /*
    for (uint32_t i = 0; i < res.size(); i++) {
      res[i]->id = i;
      print_file(*res[i], myFile);
    }
  */
  std::cout << "Found " << res.size() << " files (reading " << n_reads_failed
            << " failed)." << std::endl;
  return 0;
}

template <typename TP>
std::time_t to_time_t(TP tp) {
  using namespace std::chrono;
  auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now() +
                                                      system_clock::now());
  return system_clock::to_time_t(sctp);
}

void print_file(const File& f, std::ofstream& filestream) {
  filestream << "{" << std::endl;
  filestream << "\t"
             << "\"id\": " << f.id << "," << std::endl;
  filestream << "\t"
             << "\"fname\": \"" << f.filename << "\"," << std::endl;
  filestream << "\t"
             << "\"suffix\": \"" << f.suffix << "\"," << std::endl;
  filestream << "\t"
             << "\"path\": \"" << f.path << "\"," << std::endl;
  filestream << "\t"
             << "\"modified\": " << f.modified << "," << std::endl;
  filestream << "\t"
             << "\"md5filehash\": \"" << f.md5filehash << "\"," << std::endl;
  filestream << "\t"
             << "\"size\": " << f.size << std::endl;
  filestream << "}," << std::endl;
}
