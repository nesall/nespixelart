#include <catch2/catch_test_macros.hpp>
#include "core.h"

// Using NES constants
using namespace core;


TEST_CASE("Screen set/get pixel works") {
  Canvas canv;
  canv.setColorAt(0, 0, clr::DarkRed);
  canv.setColorAt(NAMETABLE_HEIGHT - 1, NAMETABLE_WIDTH - 1, clr::DarkBlue);
  REQUIRE(canv.colorAt(0, 0) == Color(clr::DarkRed));
  REQUIRE(canv.colorAt(NAMETABLE_HEIGHT - 1, NAMETABLE_WIDTH - 1) == Color(clr::DarkBlue));
}

TEST_CASE("Pixel to tile; Tile to pixel") {
  clri_t pixels[8][8] = {
    {0, 1, 2, 3, 0, 1, 2, 3},
    {1, 0, 3, 2, 1, 0, 3, 2},
    {2, 3, 0, 1, 2, 3, 0, 1},
    {3, 2, 1, 0, 3, 2, 1, 0},
    {0, 1, 2, 3, 0, 1, 2, 3},
    {1, 0, 3, 2, 1, 0, 3, 2},
    {2, 3, 0, 1, 2, 3, 0, 1},
    {3, 2, 1, 0, 3, 2, 1, 0}
  };
  Tile tile = Tile::pixelsToTile(pixels);
  uint8_t outPixels[8][8]{ 0 };
  Tile::tileToPixels(tile, outPixels);
  for (int y = 0; y < TILE_SIZE; ++y) {
    for (int x = 0; x < TILE_SIZE; ++x) {
      REQUIRE(outPixels[y][x] == pixels[y][x]);
    }
  }
}

TEST_CASE("Tile slicing produces correct number of tiles") {
  Canvas canv;

  // Fill top-left 8x8 with RED
  for (int y = 0; y < TILE_SIZE; ++y)
    for (int x = 0; x < TILE_SIZE; ++x)
      canv.setColorAt(y, x, clr::DarkRed);

  auto tileSet = Tilizer::sliceScreen(canv);

  // First tile should exist
  REQUIRE(!tileSet.empty());

  // Deduplication: single tile
  REQUIRE(tileSet.size() == 1);

  // Tile plane check placeholder
  // REQUIRE(tiles[0].planes[0] == ...);
}

TEST_CASE("Attribute table generated correctly") {
  Canvas canv;
  BgPalettes bgp;
  // Fill top-left 16x16 block with RED
  for (int y = 0; y < 2 * TILE_SIZE; ++y)
    for (int x = 0; x < 2 * TILE_SIZE; ++x)
      canv.setColorAt(y, x, clr::DarkRed);

  bgp.palettes[0][0] = clr::DarkRed;
  bgp.palettes[0][1] = clr::DarkGreen;
  bgp.palettes[0][2] = clr::DarkBlue;
  bgp.palettes[0][3] = clr::White;

  auto attr_table = Tilizer::generateAttributeTable(canv, bgp);

  // Each 16x16 block should pick dominant palette index (0 here)
  REQUIRE(attr_table.bytes[0] == 0);
}

TEST_CASE("Tile validation catches >4 colors") {
  Canvas canv;
  BgPalettes bgp;
  bgp.palettes[0][0] = clr::DarkRed;
  bgp.palettes[0][1] = clr::DarkGreen;
  bgp.palettes[0][2] = clr::DarkBlue;
  bgp.palettes[0][3] = clr::White;

  // Fill 8x8 tile with 5 colors
  for (int y = 0; y < TILE_SIZE; ++y)
    for (int x = 0; x < TILE_SIZE; ++x)
      canv.setColorAt(y, x, x); // 0..7 colors, simplified

  auto errors = Validator::validate(canv, bgp);

  REQUIRE(!errors.empty());
  REQUIRE(errors[0].type == TileError::TooManyColors);
}

TEST_CASE("Tile validation for invalid palette") {
  Canvas canv;
  BgPalettes bgp;
  bgp.palettes[0][0] = clr::Black;
  bgp.palettes[0][1] = clr::DarkGreen;
  bgp.palettes[0][2] = clr::DarkBlue;
  bgp.palettes[0][3] = clr::DarkRed;
  bgp.palettes[1][0] = clr::Black;
  bgp.palettes[1][1] = clr::DarkBrown;
  bgp.palettes[1][2] = clr::DarkOlive;
  bgp.palettes[1][3] = clr::White;

  // Fill 8x8 tile with 5 colors
  for (int y = 0; y < TILE_SIZE; ++y)
    for (int x = 0; x < TILE_SIZE; ++x)
      canv.setColorAt(y, x, x); // 0..7 colors, simplified

  auto errors = Validator::validate(canv, bgp);

  REQUIRE(!errors.empty());
  REQUIRE(errors[0].type == TileError::TooManyColors);
}

TEST_CASE("Deduplication merges identical tiles") {
  Canvas canv;

  // Two identical 8x8 tiles
  for (int y = 0; y < TILE_SIZE; ++y)
    for (int x = 0; x < TILE_SIZE; ++x)
      canv.setColorAt(y, x, clr::DarkRed);

  auto tileSet = Tilizer::sliceScreen(canv);

  // Only one unique tile in tileset
  REQUIRE(tileSet.size() == 1);
}

TEST_CASE("CHR, Nametable, and Attribute export produces correct size") {
  Canvas canv;
  BgPalettes bgp;

  auto nesScreen = Tilizer::generate(canv, bgp);

  auto chr_bytes = Exporter::buildCHR(nesScreen.tiles);
  auto nametable_bytes = Exporter::buildNametable(nesScreen.nametable);
  auto attr_bytes = Exporter::buildAttribute(nesScreen.attributes);

  REQUIRE(chr_bytes.size() <= 4096); // 256 tiles max
  REQUIRE(nametable_bytes.size() == NAMETABLE_WIDTH * NAMETABLE_HEIGHT);
  REQUIRE(attr_bytes.size() == ATTRIBUTE_WIDTH * ATTRIBUTE_HEIGHT);
}
