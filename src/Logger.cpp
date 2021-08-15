#include "Logger.h"

Logger::Logger(const char* filename) {
  this->filename = filename;
  this->file = new std::ofstream(filename);
}

Logger::~Logger() {
  this->file->close();
  delete this->file;
}

void Logger::write_log_entry(const char* value) {
  *(this->file) << "LOL" << value;
}
