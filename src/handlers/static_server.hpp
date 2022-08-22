#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_context.hpp>
#include <userver/components/component_list.hpp>
#include <userver/dynamic_config/snapshot.hpp>
#include <userver/dynamic_config/source.hpp>
#include <userver/dynamic_config/storage/component.hpp>
#include <userver/dynamic_config/value.hpp>
#include <userver/formats/json/value.hpp>
#include <userver/http/content_type.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

#include "../components/static_storage.hpp"

namespace static_server {

class StaticServerHandler final
    : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-static-server";

  using HttpHandlerBase::HttpHandlerBase;

  StaticServerHandler(const userver::components::ComponentConfig& config,
                      const userver::components::ComponentContext& context);

  std::string GetContentType(const FileInfo& file) const;

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override;

 private:
  const static_server::StaticStorage& storage_;
  userver::dynamic_config::Source config_;
};

std::string SayHelloTo(std::string_view name);

void AppendHello(userver::components::ComponentList& component_list);

}  // namespace static_server
