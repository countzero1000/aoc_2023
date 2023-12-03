
#include <absl/strings/str_cat.h>
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

#include "solution.h"

class Day01Solution : public Solution {

  std::string file_name() override { return "inputs/day_01.txt"; }

  void find_all(const std::string &main, const std::string &target,
                std::vector<int> &out_vector) {
    out_vector.clear();
    size_t cur_index = 0;
    while (cur_index != std::string::npos && cur_index < main.size()) {
      cur_index = main.find(target, cur_index);
      if (cur_index == std::string::npos) {
        break;
      }
      out_vector.push_back(cur_index);
      cur_index += 1;
    }
  }

  absl::StatusOr<std::string> SolveB(std::ifstream &input_file) override {

    int calibration_sum = 0;
    std::string line;
    std::vector<std::string> spelled = {"one", "two",   "three", "four", "five",
                                        "six", "seven", "eight", "nine"};

    while (std::getline(input_file, line)) {

      std::map<int, int> locations;
      std::vector<int> out_vector;
      for (size_t i = 0; i < spelled.size(); i++) {
        const std::string &sp = spelled[i];
        find_all(line, sp, out_vector);
        for (int index : out_vector) {
          locations[index] = i + 1;
        }
      }

      int first = -1;
      int last = -1;

      for (size_t i = 0; i < line.size(); i++) {

        char ch = line.at(i);
        int val = -1;

        if (std::isdigit(ch)) {
          val = (int)(ch - '0');
        } else {
          auto iter = locations.find(i);
          if (iter != locations.end()) {
            val = iter->second;
          }
        }
        if (val == -1)
          continue;
        if (first == -1) {
          first = val;
          last = val;
        } else {
          last = val;
        }
      }
      calibration_sum += first * 10 + last;
    }
    return absl::StrCat("", calibration_sum);
  };

  absl::StatusOr<std::string> SolveA(std::ifstream &input_file) override {
    std::string line;
    int calibration_sum = 0;
    while (std::getline(input_file, line)) {
      int first = -1;
      int last = -1;
      for (size_t i = 0; i < line.size(); i++) {
        char ch = line[i];
        if (std::isdigit(ch)) {
          int val = (int)(ch - '0');
          if (first == -1) {
            first = val;
            last = val;
          } else {
            last = val;
          }
        }
      }
      calibration_sum += first * 10 + last;
    };
    return absl::StrCat("", calibration_sum);
  }
};

int main() {
  Day01Solution solution;
  absl::Status status = solution.Run();
  if (status.ok()) {
    std::cout << "success" << std::endl;
  }
  return 0;
}
