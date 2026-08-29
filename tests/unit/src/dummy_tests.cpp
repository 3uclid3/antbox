#include <antbox/dummy.hpp>
#include <doctest/doctest.h>

namespace antbox { namespace {

TEST_CASE("dummy")
{
    CHECK_NOTHROW(dummy());
}

}} // namespace antbox
