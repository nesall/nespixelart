#include "core.h"
#include <algorithm>


core::Tile core::Tile::pixelsToTile(const clri_t pixels[TILE_SIZE][TILE_SIZE])
{
  Tile tile{};
  for (int y = 0; y < TILE_SIZE; ++y) {
    uint8_t p0 = 0;
    uint8_t p1 = 0;
    for (int x = 0; x < TILE_SIZE; ++x) {
      uint8_t color = pixels[y][x] & 0x03; // mask 2 bits
      p0 |= ((color & 0x01) << (7 - x)); // bit 0
      p1 |= (((color & 0x02) >> 1) << (7 - x)); // bit 1
    }
    tile.plane0[y] = p0;
    tile.plane1[y] = p1;
  }
  return tile;
}

void core::Tile::tileToPixels(const Tile &tile, clri_t pixels[TILE_SIZE][TILE_SIZE])
{
  for (int y = 0; y < TILE_SIZE; ++y) {
    uint8_t p0 = tile.plane0[y];
    uint8_t p1 = tile.plane1[y];
    for (int x = 0; x < TILE_SIZE; ++x) {
      uint8_t bit0 = (p0 >> (7 - x)) & 1;
      uint8_t bit1 = (p1 >> (7 - x)) & 1;
      pixels[y][x] = (bit1 << 1) | bit0;
    }
  }
}


//-------------------------------------------------------------------------------

bool core::TileSet::addTile(const Tile &t)
{
  if (std::find(tiles_.begin(), tiles_.end(), t) == tiles_.end()) {
    tiles_.push_back(t);
    return true;
  }
  return false;
}