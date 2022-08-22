#include "static_server.hpp"

#include <fmt/format.h>

namespace static_server {

namespace {

userver::formats::json::Value ParseRuntimeCfg(
    const userver::dynamic_config::DocsMap& docs_map) {
  return docs_map.Get("STATIC_SERVER_CONTENT_TYPE_MAP");
}
constexpr userver::dynamic_config::Key<ParseRuntimeCfg> kContentTypeMap{};
}  // namespace

StaticServerHandler::StaticServerHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<static_server::StaticStorage>()),
      config_(context.FindComponent<userver::components::DynamicConfig>()
                  .GetSource()) {}

std::string StaticServerHandler::GetContentType(const FileInfo& file) const {
  auto snap = config_.GetSnapshot();
  auto map = snap[kContentTypeMap];
  auto def = map["__default__"].As<std::string>("taxt/plain");
  return map[file.content_type].As<std::string>(def);
}

std::string StaticServerHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&) const {
  try {
    LOG_INFO() << "Handler: " << request.GetRequestPath();
    auto file = storage_.GetData(request.GetRequestPath());
    LOG_INFO() << "ok";
    request.GetHttpResponse().SetContentType(
        userver::http::ContentType(GetContentType(file)));
    return file.data;
  } catch (std::exception& e) {
    request.GetResponse().SetStatusNotFound();
    return "File not found";
  }
}

}  // namespace static_server
