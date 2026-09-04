#pragma once

#include <array>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <unordered_map>
#include <utility>
#include <vector>

#include <ant/detail/core/assert.hpp>

#include <antbox/core/vec2.hpp>

namespace antbox {

struct grid_coordinate
{
    std::int64_t x{};
    std::int64_t y{};

    auto operator==(const grid_coordinate&) const -> bool = default;
};

struct grid_region
{
    grid_coordinate min{};
    grid_coordinate max{}; // exclusive
};

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
class basic_spatial_grid
{
public:
    using cell_type = Cell;
    using coordinate_type = grid_coordinate;
    using region_type = grid_region;

public:
    static constexpr std::size_t cell_size = CellSize;
    static constexpr std::size_t chunk_extent = ChunkExtent;
    static constexpr std::size_t chunk_size = CellSize * ChunkExtent;

    static_assert(CellSize > 0);
    static_assert(ChunkExtent > 0);
    static_assert(ChunkExtent <= std::numeric_limits<std::size_t>::max() / ChunkExtent);
    static_assert(CellSize <= std::numeric_limits<std::size_t>::max() / ChunkExtent);
    static_assert(ChunkExtent <= static_cast<std::size_t>(std::numeric_limits<std::int64_t>::max()));

public:
    [[nodiscard]] static auto coordinate_at(vec2 world_position) noexcept -> coordinate_type;
    [[nodiscard]] static auto chunk_coordinate_at(coordinate_type cell) noexcept -> coordinate_type;
    [[nodiscard]] static auto cell_origin(coordinate_type cell) noexcept -> vec2;

public:
    [[nodiscard]] auto contains(coordinate_type cell) const noexcept -> bool;

    // Returns nullptr when the containing chunk has not been allocated.
    [[nodiscard]] auto find(coordinate_type cell) const noexcept -> const Cell*;
    [[nodiscard]] auto find(coordinate_type cell) noexcept -> Cell*;

    // Allocates and initializes the containing chunk when necessary.
    auto ensure(coordinate_type cell) -> Cell&;

    // Visits cells in allocated chunks only and never expands the grid.
    template<typename Function>
    requires std::invocable<Function&, coordinate_type, Cell&>
    void visit_existing(region_type region, Function&& function);

    auto erase_chunk(coordinate_type chunk_coordinate) noexcept -> bool;

    void clear() noexcept;

    [[nodiscard]] auto chunk_count() const noexcept -> std::size_t;
    [[nodiscard]] auto empty() const noexcept -> bool;

private:
    using index_type = std::size_t;

    static constexpr std::size_t chunk_cell_count = ChunkExtent * ChunkExtent;

    struct chunk
    {
        explicit chunk(coordinate_type coordinate);

        coordinate_type coordinate;
        std::array<Cell, chunk_cell_count> cells{};
    };

    struct coordinate_hash
    {
        [[nodiscard]] auto operator()(coordinate_type coordinate) const noexcept -> std::size_t;
    };

private:
    [[nodiscard]] static constexpr auto floor_divide(std::int64_t value, std::size_t divisor) noexcept -> std::int64_t;
    [[nodiscard]] static constexpr auto cell_index_at(coordinate_type cell, coordinate_type chunk) noexcept -> index_type;

    [[nodiscard]] auto find_chunk(coordinate_type chunk_coordinate) const noexcept -> const chunk*;
    [[nodiscard]] auto find_chunk(coordinate_type chunk_coordinate) noexcept -> chunk*;
    auto ensure_chunk(coordinate_type chunk_coordinate) -> chunk&;

private:
    std::vector<chunk> _chunks;
    std::unordered_map<coordinate_type, index_type, coordinate_hash> _chunk_indices;
};

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
basic_spatial_grid<Cell, CellSize, ChunkExtent>::chunk::chunk(coordinate_type coordinate)
    : coordinate{coordinate}
{
}

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
auto basic_spatial_grid<Cell, CellSize, ChunkExtent>::coordinate_hash::operator()(coordinate_type coordinate) const noexcept -> std::size_t
{
    constexpr auto golden_ratio = static_cast<std::size_t>(0x9e3779b97f4a7c15ULL);
    const std::size_t x = std::hash<std::int64_t>{}(coordinate.x);
    const std::size_t y = std::hash<std::int64_t>{}(coordinate.y);
    return x ^ (y + golden_ratio + (x << 6U) + (x >> 2U));
}

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
auto basic_spatial_grid<Cell, CellSize, ChunkExtent>::coordinate_at(vec2 world_position) noexcept -> coordinate_type
{
    const float size = static_cast<float>(CellSize);
    return {
        .x = static_cast<std::int64_t>(std::floor(world_position.x / size)),
        .y = static_cast<std::int64_t>(std::floor(world_position.y / size))};
}

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
auto basic_spatial_grid<Cell, CellSize, ChunkExtent>::chunk_coordinate_at(coordinate_type cell) noexcept -> coordinate_type
{
    return {
        .x = floor_divide(cell.x, ChunkExtent),
        .y = floor_divide(cell.y, ChunkExtent)};
}

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
auto basic_spatial_grid<Cell, CellSize, ChunkExtent>::cell_origin(coordinate_type cell) noexcept -> vec2
{
    return {
        .x = static_cast<float>(cell.x) * static_cast<float>(CellSize),
        .y = static_cast<float>(cell.y) * static_cast<float>(CellSize)};
}

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
auto basic_spatial_grid<Cell, CellSize, ChunkExtent>::contains(coordinate_type cell) const noexcept -> bool
{
    return find(cell) != nullptr;
}

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
auto basic_spatial_grid<Cell, CellSize, ChunkExtent>::find(coordinate_type cell) const noexcept -> const Cell*
{
    const coordinate_type chunk_coordinate = chunk_coordinate_at(cell);
    const chunk* found = find_chunk(chunk_coordinate);
    return found != nullptr ? &found->cells[cell_index_at(cell, chunk_coordinate)] : nullptr;
}

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
auto basic_spatial_grid<Cell, CellSize, ChunkExtent>::find(coordinate_type cell) noexcept -> Cell*
{
    return const_cast<Cell*>(std::as_const(*this).find(cell));
}

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
auto basic_spatial_grid<Cell, CellSize, ChunkExtent>::ensure(coordinate_type cell) -> Cell&
{
    const coordinate_type chunk_coordinate = chunk_coordinate_at(cell);
    chunk& containing_chunk = ensure_chunk(chunk_coordinate);
    return containing_chunk.cells[cell_index_at(cell, chunk_coordinate)];
}

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
template<typename Function>
requires std::invocable<Function&, grid_coordinate, Cell&>
void basic_spatial_grid<Cell, CellSize, ChunkExtent>::visit_existing(region_type region, Function&& function)
{
    if (region.min.x >= region.max.x || region.min.y >= region.max.y)
    {
        return;
    }

    for (chunk& existing_chunk : _chunks)
    {
        for (std::size_t local_y = 0; local_y < ChunkExtent; ++local_y)
        {
            for (std::size_t local_x = 0; local_x < ChunkExtent; ++local_x)
            {
                const coordinate_type cell{
                    .x = existing_chunk.coordinate.x * static_cast<std::int64_t>(ChunkExtent) + static_cast<std::int64_t>(local_x),
                    .y = existing_chunk.coordinate.y * static_cast<std::int64_t>(ChunkExtent) + static_cast<std::int64_t>(local_y)};

                if (cell.x >= region.min.x && cell.x < region.max.x && cell.y >= region.min.y && cell.y < region.max.y)
                {
                    std::invoke(function, cell, existing_chunk.cells[local_x + local_y * ChunkExtent]);
                }
            }
        }
    }
}

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
auto basic_spatial_grid<Cell, CellSize, ChunkExtent>::erase_chunk(coordinate_type chunk_coordinate) noexcept -> bool
{
    const auto found = _chunk_indices.find(chunk_coordinate);
    if (found == _chunk_indices.end())
    {
        return false;
    }

    const index_type index = found->second;
    if (index != _chunks.size() - 1)
    {
        std::swap(_chunks[index], _chunks.back());

        const auto moved = _chunk_indices.find(_chunks[index].coordinate);
        ANT_ASSERT(moved != _chunk_indices.end(), "moved spatial grid chunk is not indexed");
        moved->second = index;
    }

    _chunks.pop_back();
    _chunk_indices.erase(found);
    return true;
}

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
void basic_spatial_grid<Cell, CellSize, ChunkExtent>::clear() noexcept
{
    _chunks.clear();
    _chunk_indices.clear();
}

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
auto basic_spatial_grid<Cell, CellSize, ChunkExtent>::chunk_count() const noexcept -> std::size_t
{
    return _chunks.size();
}

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
auto basic_spatial_grid<Cell, CellSize, ChunkExtent>::empty() const noexcept -> bool
{
    return _chunks.empty();
}

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
constexpr auto basic_spatial_grid<Cell, CellSize, ChunkExtent>::floor_divide(std::int64_t value, std::size_t divisor) noexcept -> std::int64_t
{
    const auto signed_divisor = static_cast<std::int64_t>(divisor);
    const std::int64_t quotient = value / signed_divisor;
    return value % signed_divisor < 0 ? quotient - 1 : quotient;
}

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
constexpr auto basic_spatial_grid<Cell, CellSize, ChunkExtent>::cell_index_at(coordinate_type cell, coordinate_type chunk) noexcept -> index_type
{
    const auto extent = static_cast<std::int64_t>(ChunkExtent);
    const coordinate_type local{
        .x = cell.x - chunk.x * extent,
        .y = cell.y - chunk.y * extent};

    ANT_ASSERT(local.x >= 0 && local.x < extent, "spatial grid cell x is outside its chunk");
    ANT_ASSERT(local.y >= 0 && local.y < extent, "spatial grid cell y is outside its chunk");

    return static_cast<index_type>(local.x + local.y * extent);
}

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
auto basic_spatial_grid<Cell, CellSize, ChunkExtent>::find_chunk(coordinate_type chunk_coordinate) const noexcept -> const chunk*
{
    const auto found = _chunk_indices.find(chunk_coordinate);
    return found != _chunk_indices.end() ? &_chunks[found->second] : nullptr;
}

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
auto basic_spatial_grid<Cell, CellSize, ChunkExtent>::find_chunk(coordinate_type chunk_coordinate) noexcept -> chunk*
{
    return const_cast<chunk*>(std::as_const(*this).find_chunk(chunk_coordinate));
}

template<typename Cell, std::size_t CellSize, std::size_t ChunkExtent>
requires std::default_initializable<Cell>
auto basic_spatial_grid<Cell, CellSize, ChunkExtent>::ensure_chunk(coordinate_type chunk_coordinate) -> chunk&
{
    const auto found = _chunk_indices.find(chunk_coordinate);
    if (found != _chunk_indices.end())
    {
        return _chunks[found->second];
    }

    const index_type index = _chunks.size();
    _chunks.emplace_back(chunk_coordinate);
    _chunk_indices.emplace(chunk_coordinate, index);
    return _chunks.back();
}

} // namespace antbox
