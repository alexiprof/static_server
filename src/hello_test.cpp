#include "hello.hpp"

#include <userver/utest/utest.hpp>

UTEST(SayHelloTo, Basic) {
  EXPECT_EQ(static_server::SayHelloTo("Developer"), "Hello, Developer!\n");
  EXPECT_EQ(static_server::SayHelloTo({}), "Hello, unknown user!\n");
}
