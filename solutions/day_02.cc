
#include <absl/container/flat_hash_map.h>
#include <absl/status/internal/status_internal.h>
#include <absl/status/status.h>
#include <absl/status/statusor.h>
#include <absl/strings/str_cat.h>
#include <absl/strings/str_format.h>
#include <absl/strings/str_split.h>
#include <absl/strings/string_view.h>
#include <absl/time/clock.h>
#include <absl/time/time.h>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "solution.h"
#include "status_macros.h"

class DayO2Solution : public Solution {
private:
  std::string file_name() override { return "inputs/day_02.txt"; }

  absl::flat_hash_map<std::string, int> default_map() {
    absl::flat_hash_map<std::string, int> map;
    map["red"] = 0;
    map["green"] = 0;
    map["blue"] = 0;
    return map;
  }

  bool inbounds(const absl::flat_hash_map<std::string, int> &constraints,
                const absl::flat_hash_map<std::string, int> &max_counts) {
    for (const std::string key : {"red", "green", "blue"}) {
      if (max_counts.at(key) > constraints.at(key)) {
        return false;
      }
    }
    return true;
  }

  absl::flat_hash_map<std::string, int> parse(absl::string_view line,
                                              int &out_id_num) {
    std::vector<std::string> game_rest = absl::StrSplit(line, ":");
    std::vector<std::string> id = absl::StrSplit(game_rest[0], " ");
    int id_num = std::atoi(id[1].c_str());
    out_id_num = id_num;
    auto map = default_map();
    std::vector<std::string> rounds = absl::StrSplit(game_rest[1], ";");
    for (const std::string &str : rounds) {
      std::vector<std::string> nums = absl::StrSplit(str, ",");
      for (const std::string &num : nums) {
        std::vector<std::string> num_color = absl::StrSplit(num, " ");
        int count = std::atoi(num_color[1].c_str());
        std::string color = num_color[2];
        map[color] = std::max(map[color], count);
      }
    }
    return map;
  }

  absl::StatusOr<std::string> SolveB(std::ifstream &input_file) override {
    int power_sum = 0;
    std::string line;
    while (std::getline(input_file, line)) {
      int id_num;
      auto max_counts = parse(line, id_num);
      int power = 1;
      for (const std::string key : {"red", "green", "blue"}) {
        power *= max_counts[key];
      }
      power_sum += power;
    }
    return absl::StrCat("", power_sum);
  };

  absl::StatusOr<std::string> SolveA(std::ifstream &input_file) override {
    int id_sum = 0;
    std::string line;
    absl::flat_hash_map<std::string, int> bag;
    bag["green"] = 13;
    bag["red"] = 12;
    bag["blue"] = 14;
    while (std::getline(input_file, line)) {
      int id_num;
      auto max_counts = parse(line, id_num);
      if (inbounds(bag, max_counts)) {
        id_sum += id_num;
      }
    }
    return absl::StrCat("", id_sum);
  };
};

int main() {
  DayO2Solution solution;
  absl::Status status = solution.Run();
  if (status.ok()) {
    std::cout << "success" << std::endl;
  }
}
