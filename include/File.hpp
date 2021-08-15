#ifndef File_H
#define File_H
#include <jsoncpp/json/value.h>

struct File {
  File();
  uint32_t id;
  std::string filename;
  std::string suffix;
  std::string defaultSuffix;
  std::string path;
  std::string filetype;
  std::string md5filehash;
  long long modified;
  long long size;
  void get_json_object(Json::Value& json_value);
};
#endif
