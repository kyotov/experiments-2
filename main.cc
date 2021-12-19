#include <glog/logging.h>

int main() {
  LOG(INFO) << "Start of program.";
  LOG(ERROR) << "Check error log.";
  LOG(INFO) << "Done.";
  return 0;
}