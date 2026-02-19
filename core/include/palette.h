#pragma once

#include <cstdint>
#include <array>
#include <stdexcept>

namespace core {

  namespace clr {

    // NES PPU palette indices (0x00–0x3F)
    // Common NTSC composite naming (community standard)

    // --- Row 0x00 ---
    inline constexpr uint8_t DarkGray = 0x00;
    inline constexpr uint8_t DarkBlue = 0x01;
    inline constexpr uint8_t DarkIndigo = 0x02;
    inline constexpr uint8_t DarkPurple = 0x03;
    inline constexpr uint8_t DarkMagenta = 0x04;
    inline constexpr uint8_t DarkRed = 0x05;
    inline constexpr uint8_t DarkOrange = 0x06;
    inline constexpr uint8_t DarkBrown = 0x07;
    inline constexpr uint8_t DarkOlive = 0x08;
    inline constexpr uint8_t DarkGreen = 0x09;
    inline constexpr uint8_t DarkTeal = 0x0A;
    inline constexpr uint8_t DarkCyan = 0x0B;
    inline constexpr uint8_t VeryDarkBlue = 0x0C;
    inline constexpr uint8_t UnstableBlack = 0x0D; // avoid (signal glitch risk)
    inline constexpr uint8_t MirrorBlack0E = 0x0E; // mirror/unused
    inline constexpr uint8_t Black = 0x0F;

    // --- Row 0x10 ---
    inline constexpr uint8_t MediumGray = 0x10;
    inline constexpr uint8_t Blue = 0x11;
    inline constexpr uint8_t BrightBlue = 0x12;
    inline constexpr uint8_t Violet = 0x13;
    inline constexpr uint8_t Purple = 0x14;
    inline constexpr uint8_t Magenta = 0x15;
    inline constexpr uint8_t Red = 0x16;
    inline constexpr uint8_t OrangeRed = 0x17;
    inline constexpr uint8_t Orange = 0x18;
    inline constexpr uint8_t Olive = 0x19;
    inline constexpr uint8_t Green = 0x1A;
    inline constexpr uint8_t Teal = 0x1B;
    inline constexpr uint8_t Cyan = 0x1C;
    inline constexpr uint8_t MirrorBlack1D = 0x1D;
    inline constexpr uint8_t MirrorBlack1E = 0x1E;
    inline constexpr uint8_t MirrorBlack1F = 0x1F;

    // --- Row 0x20 ---
    inline constexpr uint8_t LightGray = 0x20;
    inline constexpr uint8_t LightBlue = 0x21;
    inline constexpr uint8_t SkyBlue = 0x22;
    inline constexpr uint8_t LightViolet = 0x23;
    inline constexpr uint8_t LightPurple = 0x24;
    inline constexpr uint8_t Pink = 0x25;
    inline constexpr uint8_t LightRed = 0x26;
    inline constexpr uint8_t LightOrange = 0x27;
    inline constexpr uint8_t Gold = 0x28;
    inline constexpr uint8_t LightOlive = 0x29;
    inline constexpr uint8_t LightGreen = 0x2A;
    inline constexpr uint8_t Aqua = 0x2B;
    inline constexpr uint8_t LightCyan = 0x2C;
    inline constexpr uint8_t MirrorBlack2D = 0x2D;
    inline constexpr uint8_t MirrorBlack2E = 0x2E;
    inline constexpr uint8_t MirrorBlack2F = 0x2F;

    // --- Row 0x30 ---
    inline constexpr uint8_t White = 0x30;
    inline constexpr uint8_t PaleBlue = 0x31;
    inline constexpr uint8_t PaleSkyBlue = 0x32;
    inline constexpr uint8_t PaleViolet = 0x33;
    inline constexpr uint8_t PalePurple = 0x34;
    inline constexpr uint8_t PalePink = 0x35;
    inline constexpr uint8_t PaleRed = 0x36;
    inline constexpr uint8_t PaleOrange = 0x37;
    inline constexpr uint8_t Yellow = 0x38;
    inline constexpr uint8_t LightYellow = 0x39;
    inline constexpr uint8_t Lime = 0x3A;
    inline constexpr uint8_t BrightGreen = 0x3B;
    inline constexpr uint8_t BrightCyan = 0x3C;
    inline constexpr uint8_t MirrorBlack3D = 0x3D;
    inline constexpr uint8_t MirrorBlack3E = 0x3E;
    inline constexpr uint8_t MirrorBlack3F = 0x3F;

  } // namespace clr

  class Color {
    uint8_t c_{};
  public:
    constexpr Color() = default;
    constexpr Color(std::uint8_t c) : c_(c) {}
    bool operator ==(const Color &other) const { return c_ == other.c_; }
    uint8_t index() const { return c_; }
  };

  template <std::size_t N>
  class Palette {
    static_assert(0 < N, "Palette size must be > 0");
  public:
    using value_type = Color;
    using storage_type = std::array<value_type, N>;
    static constexpr std::size_t size_v = N;
    constexpr Palette() = default;
    constexpr explicit Palette(const storage_type &colors) : colors_(colors) {}
    [[nodiscard]] static constexpr std::size_t size() noexcept { return N; }
    constexpr value_type &operator[](std::size_t i) noexcept { return colors_[i]; }
    constexpr const value_type &operator[](std::size_t i) const noexcept { return colors_[i]; }
    constexpr value_type &at(std::size_t i) { if (N <= i) throw std::out_of_range("Palette::at"); return colors_[i]; }
    constexpr const value_type &at(std::size_t i) const { if (N <= i) throw std::out_of_range("Palette::at"); return colors_[i]; }
    constexpr storage_type &data() noexcept { return colors_; }
    constexpr const storage_type &data() const noexcept { return colors_; }
    constexpr auto begin() noexcept { return colors_.begin(); }
    constexpr auto end() noexcept { return colors_.end(); }
    constexpr auto begin() const noexcept { return colors_.begin(); }
    constexpr auto end() const noexcept { return colors_.end(); }

  private:
    storage_type colors_{};
  };

  constexpr int MASTER_PALETTE_SIZE = 53; // NES master palette size, only usable colors

  inline Palette<MASTER_PALETTE_SIZE> standardNesPalette() {
    return Palette<MASTER_PALETTE_SIZE>({
      Color(clr::DarkGray), Color(clr::DarkBlue), Color(clr::DarkIndigo), Color(clr::DarkPurple),
      Color(clr::DarkMagenta), Color(clr::DarkRed), Color(clr::DarkOrange), Color(clr::DarkBrown),
      Color(clr::DarkOlive), Color(clr::DarkGreen), Color(clr::DarkTeal), Color(clr::DarkCyan),
      Color(clr::VeryDarkBlue), Color(clr::Black), Color(clr::MediumGray), Color(clr::Blue),
      Color(clr::BrightBlue), Color(clr::Violet), Color(clr::Purple),  Color(clr::Magenta),
      Color(clr::Red), Color(clr::OrangeRed), Color(clr::Orange), Color(clr::Olive),
      Color(clr::Green), Color(clr::Teal), Color(clr::Cyan), Color(clr::LightGray),
      Color(clr::LightBlue), Color(clr::SkyBlue), Color(clr::LightViolet), Color(clr::LightPurple),
      Color(clr::Pink), Color(clr::LightRed), Color(clr::LightOrange), Color(clr::Gold),
      Color(clr::LightOlive), Color(clr::LightGreen), Color(clr::Aqua), Color(clr::LightCyan),
      Color(clr::White), Color(clr::PaleBlue), Color(clr::PaleSkyBlue), Color(clr::PaleViolet),
      Color(clr::PalePurple), Color(clr::PalePink), Color(clr::PaleRed), Color(clr::PaleOrange),
      Color(clr::Yellow), Color(clr::LightYellow), Color(clr::Lime), Color(clr::BrightGreen),
      Color(clr::BrightCyan)
      });
  }

}