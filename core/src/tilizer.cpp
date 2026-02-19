#include "core.h"
#include <unordered_map>

/*
1. SLICE into 8×8 tiles
   ├─ For each 16×16 block (2×2 tiles):
   │  ├─ Extract 4 tiles (top-left, top-right, bottom-left, bottom-right)
   │  └─ Collect all unique colors in the block
   ├─ Validate: ≤4 unique colors per block
   │  └─ If >4: STOP, report error to user
   └─ Result: 32×30 = 960 tiles, each with 0-3 color indices

2. DEDUPLICATE tiles
   ├─ Hash each 8×8 tile (16 bytes of 2bpp data)
   ├─ Keep only unique tiles → CHR-ROM (max 256 tiles)
   └─ Build mapping: (tileX, tileY) → tile index in CHR-ROM

3. BUILD NAMETABLE
   ├─ For each 8×8 position (32×30 grid):
   │  └─ Store deduplicated tile index (0-255)
   └─ Result: 960-byte nametable

4. BUILD ATTRIBUTE TABLE
   ├─ For each 16×16 block (16×15 blocks):
   │  ├─ Get the 4 unique colors from step 1
   │  ├─ Find best matching palette from BgPalettes
   │  └─ Store palette index (0-3) in attribute byte
   └─ Result: 64-byte attribute table

5. CONVERT pixel values
   ├─ For each 8×8 tile:
   │  ├─ Map master palette indices → palette slot indices (0-3)
   │  ├─ Convert pixels to 2bpp planar format
   │  └─ Store in TileSet
   └─ Result: deduplicated CHR-ROM data

OUTPUT: NesScreen {nametable, tiles, attributes, palettes}
*/

//// Custom hash for std::array<uint8_t, 16>
//namespace std {
//  template <>
//  struct hash<std::array<uint8_t, 16>> {
//    size_t operator()(const std::array<uint8_t, 16> &arr) const noexcept {
//      // FNV-1a 64-bit hash
//      const size_t fnv_offset = 1469598103934665603ULL;
//      const size_t fnv_prime = 1099511628211ULL;
//      size_t hash = fnv_offset;
//      for (uint8_t byte : arr) {
//        hash ^= static_cast<size_t>(byte);
//        hash *= fnv_prime;
//      }
//      return hash;
//    }
//  };
//}//namespace std

namespace {

} // anonymous namespace

core::NesScreen core::Tilizer::generate(const Canvas &project, const BgPalettes &)
{
  NesScreen ns;
  //ns.tiles = sliceScreen(project.screen);
  //ns.attributes = generateAttributeTable(project.screen, project.palettes);
  // TODO: populate nametable indices
  return ns;
}

core::TileSet core::Tilizer::sliceScreen(const Canvas &canvas)
{
  TileSet ts;
  // TODO: implement proper 8x8 slicing & deduplication

  // 32x30 tiles
  //for (int ty = 0; ty < NAMETABLE_HEIGHT; ++ty) {
  //  for (int tx = 0; tx < NAMETABLE_WIDTH; ++tx) {
  //    Tile t = blockToTile(canvas, tx, ty);

  //    // TODO: add deduplication logic
  //    // Example: check dedupMap, insert if new, else skip
  //    //ts.tiles.push_back(t);
  //  }
  //}

  auto errors = Validator::validate(canvas, BgPalettes{}); // TODO: pass actual palettes
  if (errors.size()) {
    //throw 
  }
  Canvas::forEachBlock<TILE_SIZE, TILE_SIZE>(
    [&](int i, int j)
    {
    }
  );
  return ts;
}

core::AttributeTable core::Tilizer::generateAttributeTable(const Canvas &, const BgPalettes &)
{
  AttributeTable table{};
  // TODO: implement 16x16 block palette assignment
  return table;
}

core::Tile core::Tilizer::blockToTile(const Canvas &canvas, int tileX, int tileY)
{
  uint8_t block[TILE_SIZE][TILE_SIZE]{};
  // Copy 8x8 block from canvas pixels
  for (int y = 0; y < TILE_SIZE; ++y) {
    for (int x = 0; x < TILE_SIZE; ++x) {
      int canvasY = tileY * 8 + y;
      int canvasX = tileX * 8 + x;
      block[y][x] = canvas.colorAt(canvasY, canvasX).index() & 0x03; // mask to 2 bits
    }
  }
  return Tile::pixelsToTile(block);
}
