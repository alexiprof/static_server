#include "static_storage.hpp"

#include <userver/fs/read.hpp>
#include <userver/testsuite/testsuite_support.hpp>

#include <boost/filesystem.hpp>
#include "userver/logging/log.hpp"

namespace static_server {

namespace {
void ParseDirectory(std::unordered_map<std::string, FileInfo>& data,
                    std::string_view dir,
                    userver::engine::TaskProcessor& processor);

void ParseDirectory(std::unordered_map<std::string, FileInfo>& data,
                    std::string_view dir,
                    userver::engine::TaskProcessor& processor) {
  for (const auto& p : boost::filesystem::directory_iterator(dir.data())) {
    switch (p.status().type()) {
      case boost::filesystem::regular_file: {
        FileInfo file{};
        file.path = p.path().string().data();
        file.data = userver::fs::ReadFileContents(processor, p.path().string());
        file.content_type = boost::filesystem::extension(file.path);
        file.size = file.data.size();
        data[file.path] = file;
        break;
      }
      case boost::filesystem::directory_file:
        ParseDirectory(data, p.path().string(), processor);
        break;
      default:
        break;
    }
  }
}

}  // namespace

const FileInfo& StaticStorage::GetData(std::string_view handler) const {
  std::string path = dir_;
  path += handler.data();
  LOG_INFO() << "Find file: " << path;
  return data_.at(path);
}

StaticStorage::StaticStorage(const userver::components::ComponentConfig& config,
                             const userver::components::ComponentContext& ctx)
    : userver::components::LoggableComponentBase(config, ctx),
      dir_(config["dir"].As<std::string>()),
      tp_(ctx.GetTaskProcessor(config["fs-task-processor"].As<std::string>())),
      update_period_{
          config["update-period"].As<std::chrono::milliseconds>(10)} {
  data_updater_.Start("static_data_storage",
                      userver::utils::PeriodicTask::Settings{update_period_},
                      [this] { UpdateStorage(); });

  data_updater_.RegisterInTestsuite(
      ctx.FindComponent<userver::components::TestsuiteSupport>()
          .GetPeriodicTaskControl());
}

void StaticStorage::UpdateStorage() {
  LOG_INFO() << "Run update";
  ParseDirectory(data_, dir_.data(), tp_);
  LOG_INFO() << "Count files:" << data_.size();
}

userver::yaml_config::Schema StaticStorage::GetStaticConfigSchema() {
  return userver::yaml_config::MergeSchemas<
      userver::components::LoggableComponentBase>(R"(
type: object
description: component storage static server
additionalProperties: false
properties:
    update-period:
        type: string
        description: time update info
    fs-task-processor:
        type: string
        description: task-processor for work with files
    dir:
        type: string
        description: path in dir for public
)");
}
}  // namespace static_server
