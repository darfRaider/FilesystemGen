#include <fstream>

class Logger {
 private:
  std::ofstream* file;
  const char* filename;
  int timestamp;
  void write_header();
  void write_footer();

 public:
  Logger(const char* filename);
  ~Logger();
  void write_log_entry(const char* value);
};
