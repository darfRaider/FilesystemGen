#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
class Logger {
 private:
  std::ofstream* file;
  std::chrono::time_point<std::chrono::system_clock> start_time;
  const char* filename;
  int timestamp;
  std::string get_datetime();

 public:
  Logger(const char* filename);
  ~Logger();
  void write_header();
  void write_footer();
  void write_log_entry(const char* value);
  void write_checkpoint(const char* current_path);
};
