#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_list.hpp>

namespace static_server {

std::string SayHelloTo(std::string_view name);

void AppendHello(userver::components::ComponentList &component_list);

} // namespace static_server
