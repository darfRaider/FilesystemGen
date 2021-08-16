#include "JsonHandler.hpp"

void JsonHandler::to_json(const std::vector<File *> &input,
                          const std::string &out_path) {
  std::ofstream file(out_path.c_str());
  Json::Value json_objects(Json::arrayValue);
  for (auto &r : input) {
    Json::Value v;
    r->get_json_object(v);
    json_objects.append(v);
  }
  Json::StyledWriter styledWriter;
  file << styledWriter.write(json_objects);
 // Json::Value AllFiles(Json::arrayValue);
  file.close();
}
