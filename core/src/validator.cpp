#include "core.h"
#include <set>

/*
├─ For each 16×16 block (2×2 tiles):
│  ├─ Extract 4 tiles (top-left, top-right, bottom-left, bottom-right)
│  └─ Collect all unique colors in the block
*/
std::vector<core::TileError> core::Validator::validate(const Canvas &canvas, const BgPalettes &bg)
{
  std::vector<TileError> errors;
  constexpr auto BlockSize = TILE_SIZE * 2;
  Canvas::forEachBlock<BlockSize, BlockSize>(
    [&](int i, int j)
    {
      // 1. Extract 4 tiles (top-left, top-right, bottom-left, bottom-right)
      // 2. Collect all unique colors in the block
      // 3. Validate: ≤4 unique colors per block
      //    └─ If >4: STOP, report error to user
      // 4. Find best matching palette from BgPalettes
      //    └─ If no match: STOP, report error to user
      std::set<uint8_t> uniqueColors;
      for (int y = 0; y < BlockSize; ++y) {
        for (int x = 0; x < BlockSize; ++x) {
          int canvasY = j * 8 + y;
          int canvasX = i * 8 + x;
          Color clr = canvas.colorAt(canvasY, canvasX);
          uniqueColors.insert(clr.index());
        }
      }
      if (NUM_CLRS_PER_PALETTE < uniqueColors.size()) {
        errors.push_back(TileError{ i, j, TileError::TooManyColors });
        //return;
      }
    }
  );

  return errors;
}
