#include <antbox/graphics/color.hpp>

#include <raylib.h>

namespace antbox {

color::operator Color() const noexcept
{
    return {r, g, b, a};
}

} // namespace antbox
