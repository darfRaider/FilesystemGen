#include "File.hpp"

File::File() { this->filetype = "UNKNOWN"; }

void File::get_json_object(Json::Value& json_value) {
  json_value["id"] = this->id;
  json_value["fname"] = this->filename;
  json_value["suffix"] = this->suffix;
  json_value["defaultSuffix"] = this->defaultSuffix;
  json_value["filetype"] = this->filetype;
  json_value["path"] = this->path;
  json_value["modified"] = this->modified;
  json_value["md5filehash"] = this->md5filehash;
  json_value["size"] = this->size;
}
