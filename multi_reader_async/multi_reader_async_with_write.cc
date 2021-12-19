#include <glog/logging.h>

#include <algorithm>
#include <chrono>
#include <future>
#include <thread>
#include <vector>

void ParallelComputeWithWrite(
    const std::vector<int> input_vec,
    std::vector<int> &output_vec) {
  std::vector<std::future<void>> futures;
  futures.reserve(input_vec.size());
  for (int i = 0; i < input_vec.size(); i++) {
    futures.push_back(
        std::async(std::launch::async, [i, &input_vec, &output_vec]() {
          std::this_thread::sleep_for(std::chrono::seconds(1));
          output_vec[i] = input_vec[i] * i * 2;
        }));
  }
  LOG(INFO) << "Performing join implicitly.";
  // The below lines can be used to perform an explicit join.
  // for (auto &fs : futures) {
  //   fs.wait();
  // }
}

void ParallelComputeWite() {
  const int num_elements = 10;
  const int default_value = 4;
  std::vector<int> input_vec(num_elements, default_value);
  auto start = std::chrono::high_resolution_clock::now();
  std::vector<int> output_vec(num_elements, 0);
  ParallelComputeWithWrite(input_vec, output_vec);
  auto max_value =
      *std::max_element(std::begin(output_vec), std::end(output_vec));
  auto end = std::chrono::high_resolution_clock::now();
  LOG(INFO) << "Output vec: ";
  for (auto value : output_vec) {
    LOG(INFO) << value;
  }
  LOG(INFO)
      << "Computed max value of " << max_value << " in "
      << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
      << " second(s).";
}

int main() {
  LOG(INFO) << "Multi reader async with write.";
  ParallelComputeWite();
  return 0;
}