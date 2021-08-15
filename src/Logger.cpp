#include "Logger.h"

Logger::Logger(const char* filename) {
  start_time = std::chrono::system_clock::now();
  filename = filename;
  file = new std::ofstream(filename);
  write_header();
}

Logger::~Logger() {
  this->file->close();
  delete this->file;
}

void Logger::write_header() {
  write_log_entry("Filesystem Reader Log");
  write_log_entry(get_datetime().c_str());
  write_log_entry("***************************");
}

void Logger::write_footer() {
  write_log_entry("***************************");
  write_log_entry(get_datetime().c_str());
  write_log_entry("Filesystem read successfully");
}

void Logger::write_log_entry(const char* value) {
  *(this->file) << value << std::endl;
}

std::string Logger::get_datetime() {
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
  return ss.str();
}

void Logger::write_checkpoint(const char* checkpoint_text) {
  std::string out = get_datetime() + " " + (std::string)checkpoint_text;
  write_log_entry(out.c_str());
}
