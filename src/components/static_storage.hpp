#pragma once

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/components/loggable_component_base.hpp>
#include <userver/utils/periodic_task.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/yaml_config/schema.hpp>

namespace static_server {

struct FileInfo {
  std::string path;
  std::string handler;
  std::string content_type;
  std::string data;
  ssize_t size;
};

class StaticStorage final : public userver::components::LoggableComponentBase {
 public:
  static constexpr std::string_view kName = "static-storage";

  StaticStorage(const userver::components::ComponentConfig& config,
                const userver::components::ComponentContext& ctx);

  void UpdateStorage();

  const FileInfo& GetData(std::string_view handler) const;

  static userver::yaml_config::Schema GetStaticConfigSchema();

 private:
  std::unordered_map<std::string, FileInfo> data_;
  userver::utils::PeriodicTask data_updater_;
  std::string dir_;
  userver::engine::TaskProcessor& tp_;
  std::chrono::milliseconds update_period_;
};

}  // namespace static_server

template <>
inline constexpr bool
    userver::components::kHasValidate<static_server::StaticStorage> = true;
