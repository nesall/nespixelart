#include "core.h"
#include <unordered_map>

// Custom hash for std::array<uint8_t, 16>
namespace std {
  template <>
  struct hash<std::array<uint8_t, 16>> {
    size_t operator()(const std::array<uint8_t, 16> &arr) const noexcept {
      // FNV-1a 64-bit hash
      const size_t fnv_offset = 1469598103934665603ULL;
      const size_t fnv_prime = 1099511628211ULL;
      size_t hash = fnv_offset;
      for (uint8_t byte : arr) {
        hash ^= static_cast<size_t>(byte);
        hash *= fnv_prime;
      }
      return hash;
    }
  };
}//namespace std

core::NesScreen core::Tilizer::generate(const Canvas &project, const BgPalettes &)
{
  NesScreen ns;
  //ns.tiles = sliceScreen(project.screen);
  //ns.attributes = generateAttributeTable(project.screen, project.palettes);
  // TODO: populate nametable indices
  return ns;
}

core::TileSet core::Tilizer::sliceScreen(const Canvas &screen)
{
  TileSet ts;
  // TODO: implement proper 8x8 slicing & deduplication
  // Optional deduplication: map hash -> index
  std::unordered_map<std::array<uint8_t, TILE_SIZE_BYTES>, size_t> dedupMap;


  // 32x30 tiles
  for (int ty = 0; ty < NAMETABLE_HEIGHT; ++ty) {
    for (int tx = 0; tx < NAMETABLE_WIDTH; ++tx) {
      Tile t = blockToTile(screen, tx, ty);

      // TODO: add deduplication logic
      // Example: check dedupMap, insert if new, else skip
      ts.tiles.push_back(t);
    }
  }

  return ts;
}

core::AttributeTable core::Tilizer::generateAttributeTable(const Canvas &, const BgPalettes &)
{
  AttributeTable table{};
  // TODO: implement 16x16 block palette assignment
  return table;
}

core::Tile core::Tilizer::blockToTile(const Canvas &screen, int tileX, int tileY)
{
  uint8_t block[TILE_HEIGHT][TILE_WIDTH]{};

  // Copy 8x8 block from screen pixels
  for (int y = 0; y < TILE_HEIGHT; ++y) {
    for (int x = 0; x < TILE_WIDTH; ++x) {
      int screenX = tileX * 8 + x;
      int screenY = tileY * 8 + y;
      //block[y][x] = screen.pixels[screenY][screenX];
    }
  }

  return Tile::pixelsToTile(block);
}
