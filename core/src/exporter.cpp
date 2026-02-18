#include "core.h"

std::vector<uint8_t> core::Exporter::buildCHR(const TileSet &)
{
    return std::vector<uint8_t>();
}

std::vector<uint8_t> core::Exporter::buildNametable(const Nametable &)
{
  return std::vector<uint8_t>();
}

std::vector<uint8_t> core::Exporter::buildAttribute(const AttributeTable &)
{
  return std::vector<uint8_t>();
}

std::vector<uint8_t> core::Exporter::buildPalette(const BgPalettes &)
{
  return std::vector<uint8_t>();
}
