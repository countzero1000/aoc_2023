workspace(name = "aoc_2023")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_google_absl",
    sha256 = "725f25f09d68a7b61c861c2b5467e662a35f3fddc95b50501f3245c95285a688",
    strip_prefix = "abseil-cpp-c16a2f43206b0235d49d4f6155f285a4d4939c58",
    urls = ["https://github.com/abseil/abseil-cpp/archive/c16a2f43206b0235d49d4f6155f285a4d4939c58.zip"],
)

http_archive(
    name = "com_google_googletest",
    strip_prefix = "googletest-011959aafddcd30611003de96cfd8d7a7685c700",
    urls = ["https://github.com/google/googletest/archive/011959aafddcd30611003de96cfd8d7a7685c700.zip"],
)

http_archive(
    name = "bazel_skylib",
    sha256 = "f7be3474d42aae265405a592bb7da8e171919d74c16f082a5457840f06054728",
    urls = ["https://github.com/bazelbuild/bazel-skylib/releases/download/1.2.1/bazel-skylib-1.2.1.tar.gz"],
)

# C++ rules for Bazel.
http_archive(
    name = "rules_cc",
    sha256 = "954b7a3efc8752da957ae193a13b9133da227bdacf5ceb111f2e11264f7e8c95",
    strip_prefix = "rules_cc-9e10b8a6db775b1ecd358d8ddd3dab379a2c29a5",
    urls = ["https://github.com/bazelbuild/rules_cc/archive/9e10b8a6db775b1ecd358d8ddd3dab379a2c29a5.zip"],
)
