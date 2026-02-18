#pragma once 

#include "palette.h"

#include <cstdint>
#include <array>
#include <vector>

/*
CANVAS (256×240 pixels)
  ↓
NAMETABLE (32×30 tiles)
  - Each entry: index to a tile in CHR-ROM (0-255)
  - Size: 32×30 = 960 bytes
  ↓
ATTRIBUTE TABLE (16×16 blocks, packed into 64 bytes)
  - Each 16×16 block (2×2 tiles) → 2-bit palette selection (0-3)
  - Size: 64 bytes
  ↓
CHR-ROM (tile data)
  - Each tile: 8×8 pixels, 2-bit color (4 colors per tile)
  - NES format: 16 bytes per tile (2 bitplanes, 8 bytes each)
  - Max 256 tiles = 4096 bytes
  ↓
PALETTES (4 background palettes)
  - Each palette: 4 colors (indices into NES master palette 0-63)
  - Total: 4×4 = 16 bytes
*/

namespace core {

  constexpr int TILE_WIDTH = 8;
  constexpr int TILE_HEIGHT = 8;
  constexpr int TILE_SIZE_BYTES = 16; // NES 2bpp format
  //constexpr int TILE_SIZE_PIXELS = TILE_WIDTH * TILE_HEIGHT;
  //constexpr int TILE_SIZE_BITS = TILE_SIZE_BYTES * 8;
  constexpr int NAMETABLE_WIDTH = 32;
  constexpr int NAMETABLE_HEIGHT = 30;
  constexpr int ATTRIBUTE_WIDTH = NAMETABLE_WIDTH / 2;
  constexpr int ATTRIBUTE_HEIGHT = NAMETABLE_HEIGHT / 2;
  constexpr int ATTRIBUTE_BYTES = 64; // 32x30 nametable divided into 16x16 attribute blocks

  constexpr int NUM_BG_PALETTES = 4;
  constexpr int NUM_CLRS_PER_PALETTE = 4;

  static constexpr int SCREEN_WIDTH = 256;
  static constexpr int SCREEN_HEIGHT = 240;

  // define a 2 bit type
  using clri_t = uint8_t; // 0..3 for NES colors

  // Raw art
  struct Canvas {
    const Palette<MASTER_PALETTE_SIZE> palette_ = standardNesPalette();
    // pixels[y][x] = 0..63 (master palette index)
    std::array<std::array<Color, SCREEN_WIDTH>, SCREEN_HEIGHT> pixels{};
    void setColorAt(uint8_t row, uint8_t col, Color clr) {
      if (SCREEN_HEIGHT <= row || SCREEN_WIDTH <= col) throw std::out_of_range("Canvas::setColorAt");
      pixels[row][col] = clr.c_;
    }
    Color colorAt(uint8_t row, uint8_t col) const {
      if (SCREEN_HEIGHT <= row || SCREEN_WIDTH <= col) throw std::out_of_range("Canvas::colorAt");
      return pixels[row][col];
    }
  };

  struct Tile {
    // NES 8x8 tile = 16 bytes (2bpp planar)
    std::array<uint8_t, TILE_SIZE_BYTES> planes{};
    bool operator==(const Tile &other) const { return planes == other.planes; }
    // pixels[y][x] = 0..3 (2-bit color index)
    static Tile pixelsToTile(const clri_t pixels[TILE_HEIGHT][TILE_WIDTH]);
    static void tileToPixels(const Tile &tile, uint8_t clri_t[TILE_HEIGHT][TILE_WIDTH]);
  };

  struct TileSet {
    std::vector<Tile> tiles;
  };
  
  struct BgPalettes {
    std::array<Palette<NUM_CLRS_PER_PALETTE>, NUM_BG_PALETTES> palettes;
  };

  struct Nametable {
    uint8_t indices[NAMETABLE_HEIGHT][NAMETABLE_WIDTH];
  };

  struct AttributeTable {
    uint8_t bytes[ATTRIBUTE_BYTES];
  };

  struct NesScreen {
    Nametable nametable;
    TileSet tiles;
    AttributeTable attributes;
    BgPalettes palettes;
  };

  class Exporter {
  public:
    static std::vector<uint8_t> buildCHR(const TileSet &);
    static std::vector<uint8_t> buildNametable(const Nametable &);
    static std::vector<uint8_t> buildAttribute(const AttributeTable &);
    static std::vector<uint8_t> buildPalette(const BgPalettes &);
  };

  struct TileError {
    int tileX = 0;
    int tileY = 0;
    enum Type {
      TooManyColors,
      InvalidPaletteMatch
    } type;
  };

  struct Validator {
    static std::vector<TileError> validate(const Canvas &, const BgPalettes &);
  };

  struct Tilizer {
    static NesScreen generate(const Canvas &, const BgPalettes &);
    static TileSet sliceScreen(const Canvas &);
    static AttributeTable generateAttributeTable(const Canvas &, const BgPalettes &);
    // Helper: convert 8x8 screen block to Tile
    static Tile blockToTile(const Canvas &screen, int tileX, int tileY);
  };
}