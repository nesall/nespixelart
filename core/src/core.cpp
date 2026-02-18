#include "core.h"


core::Tile core::Tile::pixelsToTile(const clri_t pixels[TILE_HEIGHT][TILE_WIDTH])
{
  Tile tile{};
  for (int y = 0; y < TILE_HEIGHT; ++y) {
    uint8_t plane0 = 0;
    uint8_t plane1 = 0;
    for (int x = 0; x < TILE_WIDTH; ++x) {
      uint8_t color = pixels[y][x] & 0x03; // mask 2 bits
      plane0 |= ((color & 0x01) << (7 - x)); // low bit
      plane1 |= ((color & 0x02) >> 1) << (7 - x); // high bit
    }
    tile.planes[y] = plane0; // bit 0 plane
    tile.planes[y + 8] = plane1; // bit 1 plane
  }
  return tile;
}

void core::Tile::tileToPixels(const Tile &tile, clri_t pixels[TILE_HEIGHT][TILE_WIDTH])
{
  for (int y = 0; y < TILE_HEIGHT; ++y) {
    uint8_t plane0 = tile.planes[y];
    uint8_t plane1 = tile.planes[y + 8];
    for (int x = 0; x < TILE_WIDTH; ++x) {
      uint8_t bit0 = (plane0 >> (7 - x)) & 1;
      uint8_t bit1 = (plane1 >> (7 - x)) & 1;
      pixels[y][x] = (bit1 << 1) | bit0;
    }
  }
}
