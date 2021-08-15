#include "FilesystemGenerator.h"

#include "Logger.h"

template <typename TP>
std::time_t to_time_t(TP tp);

void print_file(const File& f, std::ofstream& filestream);
void print_json_file(const std::vector<File*>& f, std::ofstream& filestream);

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
}

void read_json(const std::string filename,
               std::map<std::string, std::string>& map) {
  std::ifstream file(filename, std::ifstream::binary);
  Json::Value data;
  file >> data;
  int n_suffixes = data.getMemberNames().size();
  for (int i = 0; i < n_suffixes; i++) {
    Json::Value key = data.getMemberNames()[i];
    std::string value = data[key.asString()].asString();
    map.insert(std::pair<std::string, std::string>(key.asString(), value));
  }
}

void read_json2(const std::string filename,
                std::map<std::string, std::vector<std::string>>& map) {
  std::ifstream file(filename, std::ifstream::binary);
  Json::Value data;
  file >> data;
  int n_suffixes = data.getMemberNames().size();
  for (int i = 0; i < n_suffixes; i++) {
    Json::Value key = data.getMemberNames()[i];
    Json::Value list = data[key.asString()];
    std::vector<std::string> vec;
    for (auto it = list.begin(); it != list.end(); it++) {
      vec.push_back(list[it.index()].asString());
    }
    map.insert(
        std::pair<std::string, std::vector<std::string>>(key.asString(), vec));
  }
}

int main(int argc, char** argv) {
  // MD5 Stuff
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

  // Default suffix map
  std::map<std::string, std::string> defaultSuffixMap;
  read_json("defaultSuffixMap.json", defaultSuffixMap);

  std::map<std::string, std::vector<std::string>> classMap;
  read_json2("fileClasses.json", classMap);

  std::sort(res.begin(), res.end(), cmp_strings);
  int id_cnt = 0;
  for (auto e : res) {
    // Set unique id
    e->id = id_cnt;
    // Set default default suffix
    std::string defaultSuffix(e->suffix);
    boost::to_lower(defaultSuffix);
    if (defaultSuffixMap.count(defaultSuffix)) {
      defaultSuffix = defaultSuffixMap[defaultSuffix];
    }
    e->defaultSuffix = defaultSuffix;
    // Set filetype
    for (auto const k : classMap) {
      for (auto const v : k.second) {
        if (std::strcmp(v.c_str(), defaultSuffix.c_str()) == 0) {
          e->filetype = k.first;
          break;
        }
      }
    }
    id_cnt++;
  }
  std::cout << "Found " << res.size() << " files (reading " << n_reads_failed
            << " failed)." << std::endl;
  std::string fname = "test.json";
  JsonHandler::to_json(res, fname);
  return 0;
}

template <typename TP>
std::time_t to_time_t(TP tp) {
  using namespace std::chrono;
  auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now() +
                                                      system_clock::now());
  return system_clock::to_time_t(sctp);
}