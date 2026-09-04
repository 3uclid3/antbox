#include <antbox/core/spatial_grid.hpp>
#include <doctest/doctest.h>

#include <utility>
#include <vector>

namespace antbox { namespace {

using grid = basic_spatial_grid<int, 4, 4>;

TEST_CASE("basic_spatial_grid::coordinate_at: maps world positions to cells using floor division")
{
    CHECK_EQ(grid::coordinate_at({0.0F, 3.99F}), grid_coordinate{0, 0});
    CHECK_EQ(grid::coordinate_at({4.0F, 7.99F}), grid_coordinate{1, 1});
    CHECK_EQ(grid::coordinate_at({-0.01F, -4.0F}), grid_coordinate{-1, -1});
    CHECK_EQ(grid::coordinate_at({-4.01F, -8.01F}), grid_coordinate{-2, -3});
}

TEST_CASE("basic_spatial_grid::chunk_coordinate_at: maps cells to chunks using floor division")
{
    CHECK_EQ(grid::chunk_coordinate_at({0, 3}), grid_coordinate{0, 0});
    CHECK_EQ(grid::chunk_coordinate_at({4, 7}), grid_coordinate{1, 1});
    CHECK_EQ(grid::chunk_coordinate_at({-1, -4}), grid_coordinate{-1, -1});
    CHECK_EQ(grid::chunk_coordinate_at({-5, -8}), grid_coordinate{-2, -2});
}

TEST_CASE("basic_spatial_grid::cell_origin: reports a cell's world-space origin")
{
    const vec2 origin = grid::cell_origin({-2, 3});
    CHECK_EQ(origin.x, doctest::Approx(-8.0F));
    CHECK_EQ(origin.y, doctest::Approx(12.0F));
}

TEST_CASE("basic_spatial_grid::find: returns nullptr without allocating a missing cell")
{
    grid cells;

    CHECK(cells.empty());
    CHECK_EQ(cells.find({0, 0}), nullptr);
    CHECK(cells.empty());
}

TEST_CASE("basic_spatial_grid::contains: reports whether the containing chunk is allocated")
{
    grid cells;

    CHECK_FALSE(cells.contains({0, 0}));

    cells.ensure({0, 0});

    CHECK(cells.contains({3, 3}));
    CHECK_FALSE(cells.contains({4, 0}));
}

TEST_CASE("basic_spatial_grid::ensure: allocates and default-initializes one complete chunk")
{
    grid cells;

    cells.ensure({0, 0}) = 10;
    cells.ensure({3, 3}) = 20;

    CHECK_EQ(cells.chunk_count(), 1);
    CHECK_EQ(*cells.find({0, 0}), 10);
    CHECK_EQ(*cells.find({3, 3}), 20);
    CHECK_EQ(*cells.find({1, 1}), 0);
}

TEST_CASE("basic_spatial_grid::ensure: keeps cells independent across chunks and signed coordinates")
{
    grid cells;

    const std::vector<std::pair<grid_coordinate, int>> values{
        {{4, 0}, 1},
        {{0, 64}, 2},
        {{-1, 0}, 3},
        {{0, -1}, 4},
        {{-1, -1}, 5},
        {{-5, -5}, 6},
    };

    for (const auto& [coordinate, value] : values)
    {
        cells.ensure(coordinate) = value;
    }

    CHECK_EQ(cells.chunk_count(), values.size());
    for (const auto& [coordinate, value] : values)
    {
        REQUIRE(cells.find(coordinate) != nullptr);
        CHECK_EQ(*cells.find(coordinate), value);
    }
}

TEST_CASE("basic_spatial_grid::ensure: allocates distant chunks without allocating the space between them")
{
    grid cells;

    cells.ensure({0, 0}) = 1;
    cells.ensure({4'000'000'000, -4'000'000'000}) = 2;

    CHECK_EQ(cells.chunk_count(), 2);
    CHECK_EQ(*cells.find({0, 0}), 1);
    CHECK_EQ(*cells.find({4'000'000'000, -4'000'000'000}), 2);
}

TEST_CASE("basic_spatial_grid::ensure: indexes negative cells relative to their chunk")
{
    grid cells;

    cells.ensure({-1, -1}) = 1;
    cells.ensure({-4, -1}) = 2;
    cells.ensure({-1, -4}) = 3;
    cells.ensure({-4, -4}) = 4;

    CHECK_EQ(cells.chunk_count(), 1);
    CHECK_EQ(*cells.find({-1, -1}), 1);
    CHECK_EQ(*cells.find({-4, -1}), 2);
    CHECK_EQ(*cells.find({-1, -4}), 3);
    CHECK_EQ(*cells.find({-4, -4}), 4);
}

TEST_CASE("basic_spatial_grid::erase_chunk: erases chunks and repairs moved dense entries")
{
    grid cells;
    cells.ensure({0, 0}) = 1;
    cells.ensure({4, 0}) = 2;
    cells.ensure({8, 0}) = 3;

    CHECK(cells.erase_chunk({1, 0}));
    CHECK_FALSE(cells.erase_chunk({1, 0}));
    CHECK_EQ(cells.chunk_count(), 2);
    CHECK_EQ(cells.find({4, 0}), nullptr);
    REQUIRE(cells.find({8, 0}) != nullptr);
    CHECK_EQ(*cells.find({8, 0}), 3);
}

TEST_CASE("basic_spatial_grid::visit_existing: visits only cells in allocated chunks and the requested region")
{
    basic_spatial_grid<int, 1, 2> cells;
    cells.ensure({0, 0}) = 1;
    cells.ensure({-1, -1}) = 2;
    cells.ensure({4, 4}) = 3;

    std::vector<grid_coordinate> visited;
    cells.visit_existing({.min = {-1, -1}, .max = {2, 2}}, [&](grid_coordinate coordinate, int&) {
        visited.push_back(coordinate);
    });

    CHECK_EQ(visited.size(), 5);
    CHECK_EQ(cells.chunk_count(), 3);
}

TEST_CASE("basic_spatial_grid::clear: releases every chunk")
{
    grid cells;
    cells.ensure({0, 0});
    cells.ensure({-1, -1});

    cells.clear();

    CHECK(cells.empty());
    CHECK_EQ(cells.chunk_count(), 0);
    CHECK_EQ(cells.find({0, 0}), nullptr);
    CHECK_EQ(cells.find({-1, -1}), nullptr);
}

}} // namespace antbox
