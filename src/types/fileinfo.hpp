#pragma once

#include <string>

namespace static_server {

struct FileInfo {
  std::string path;
  std::string handler;
  std::string content_type;
  ssize_t size;
};

}  // namespace static_server
