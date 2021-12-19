#include <glog/logging.h>

#include <chrono>
#include <future>
#include <limits>
#include <thread>
#include <vector>

int Compute(const std::vector<int> &input_vec, int thread_id) {
  LOG_ASSERT(thread_id < input_vec.size()) << "Invalid thread id " << thread_id;
  int running_sum = 0;
  for (int i = 0; i <= thread_id; i++) {
    running_sum += input_vec[i];
  }
  std::this_thread::sleep_for(std::chrono::seconds(1));
  LOG(INFO) << "Thread " << thread_id << " computed " << running_sum;
  return running_sum;
}

int ParallelComputeWithRead(const std::vector<int> &input_vec) {
  std::vector<std::future<int>> return_values_fs;
  // Interestingly, calling push_back without reserve triggers clang-tidy check:
  //   error: 'push_back' is called inside a loop; consider pre-allocating the
  //     container capacity before the loop
  //     [performance-inefficient-vector-operation,-warnings-as-errors]
  return_values_fs.reserve(input_vec.size());
  for (int i = 0; i < input_vec.size(); i++) {
    return_values_fs.push_back(
        std::async(std::launch::async, Compute, input_vec, i));
  }
  LOG(INFO) << "Performing parallel compute for " << input_vec.size()
            << " values";
  // As expected there is a compiler error if the below line is uncommented
  // input_vec.push_back(0);
  int max_value = std::numeric_limits<int>::min();
  for (auto &value_fs : return_values_fs) {
    auto value = value_fs.get();
    LOG(INFO) << "Read thread output value: " << value;
    max_value = std::max(value, max_value);
  }
  return max_value;
}

void ParallelComputeReadOnly() {
  const int num_elements = 10;
  const int default_value = 4;
  std::vector<int> vec(num_elements, default_value);
  auto start = std::chrono::high_resolution_clock::now();
  auto max_value = ParallelComputeWithRead(vec);
  auto end = std::chrono::high_resolution_clock::now();
  LOG(INFO)
      << "Computed max value of " << max_value << " in "
      << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
      << " second(s).";
}

int main() {
  LOG(INFO) << "Multi reader async.";
  ParallelComputeReadOnly();
  return 0;
}