#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>

#include <fstream>
#include <string>
#include <vector>

#include "File.hpp"

class JsonHandler {
public:
  static void to_json(const std::vector<File *> &input, const std::string &out_path);
};
