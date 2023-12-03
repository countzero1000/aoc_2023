
#include "status_macros.h"
#include <absl/container/flat_hash_map.h>
#include <absl/functional/any_invocable.h>
#include <absl/status/status.h>
#include <absl/status/statusor.h>
#include <absl/strings/string_view.h>
#include <absl/time/clock.h>
#include <absl/time/time.h>
#include <fstream>
#include <string>

class Solution {

public:
  Solution() {}

  absl::Status Run() {
    RETURN_IF_ERROR(RunSol());
    return absl::OkStatus();
  }

private:
  absl::Status RunSol() {
    {
      std::ifstream file = std::ifstream(file_name());
      absl::Time start = absl::Now();
      ASSIGN_OR_RETURN(std::string sol, SolveA(file));
      std::cout << "Solution A: " << sol << std::endl;
      std::cout << "Solution A took: " << absl::Now() - start << std::endl;
      file.close();
    }
    {
      std::ifstream file = std::ifstream(file_name());
      absl::Time start = absl::Now();
      ASSIGN_OR_RETURN(std::string sol, SolveB(file));
      std::cout << "Solution B: " << sol << std::endl;
      std::cout << "Solution B took: " << absl::Now() - start << std::endl;
      file.close();
    }
    return absl::OkStatus();
  }

  virtual absl::StatusOr<std::string> SolveA(std::ifstream &input_file) = 0;
  virtual absl::StatusOr<std::string> SolveB(std::ifstream &input_file) = 0;
  virtual std::string file_name() = 0;
};