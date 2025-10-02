#ifndef GOSDCLOCK_CONFIG_HPP
#define GOSDCLOCK_CONFIG_HPP

#include <string>
#include <array>
#include <filesystem>

namespace GOSDClock_Config {

  /**
   * Enum class representing on-screen display (OSD) position
   */
  enum class OSDPosition : int {
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
    Count // Number of available positions
  };

  /**
   * String labels for each OSD position
   */
  constexpr std::array<const char*, static_cast<int>(OSDPosition::Count)> OSD_POSITION_LABELS = {
    "Top Left",
    "Top Right",
    "Bottom Left",
    "Bottom Right"
  };

  /**
   * Enum class representing supported time formats.
   */
  enum class TimeFormat : int {
    TwelveHour,
    TwelveHourWithSeconds,
    TwentyFourHour,
    TwentyFourHourWithSeconds,
    Count // Number of available formats
  };

  /**
   * Human-readable labels for each time format.
   */
  constexpr std::array<const char*, static_cast<int>(TimeFormat::Count)> TIME_FORMAT_LABELS = {
    "12 Hours",
    "12 Hours (with seconds)",
    "24 Hours",
    "24 Hours (with seconds)"
  };

  /**
   * strftime-compatible format placeholders for each time format.
   */
  constexpr std::array<const char*, static_cast<int>(TimeFormat::Count)> TIME_FORMAT_PATTERNS = {
    "%I:%M %p",       // 12-hours
    "%I:%M:%S %p",    // 12-hours (with seconds)
    "%H:%M",          // 24-hours
    "%H:%M:%S"        // 24-hours (with seconds)
  };

  /**
   * Configuration class for the OSD clock.
   * Stores user preferences like position, font, and time format.
   */
  class Config {
  private:
    std::filesystem::path config_file_path;

    /* Position offsets (in pixels) */
    int horizontal_offset;  
    int vertical_offset;    

    /* OSD window position */
    OSDPosition osd_position;

    /* Font configuration */
    int font_size_px;
    std::string font_family_name;

    /* Time formatting */
    TimeFormat time_format_type;
    std::string time_format_pattern;

  public:
    Config();
    ~Config() = default;

    /* Getters Function */
    inline int get_font_size() const { return font_size_px; }
    inline int get_offset_x() const { return horizontal_offset; }
    inline int get_offset_y() const { return vertical_offset; }
    inline OSDPosition get_osd_position() const { return osd_position; }
    inline TimeFormat get_time_format() const { return time_format_type; }
    inline const std::string& get_time_format_pattern() const { return time_format_pattern; }
    inline const std::string& get_font_family() const { return font_family_name; }
    inline const std::filesystem::path& get_config_path() const { return config_file_path; }

    /* Setters Functions */
    inline void set_font_size(int new_size) { font_size_px = new_size; }
    inline void set_offset_x(int new_offset) { horizontal_offset = new_offset; }
    inline void set_offset_y(int new_offset) { vertical_offset = new_offset; }
    inline void set_osd_position(OSDPosition new_position) { osd_position = new_position; }
    inline void set_time_format(TimeFormat new_format) { time_format_type = new_format; }
    inline void set_time_format_pattern(const std::string& new_pattern) { time_format_pattern = new_pattern; }
    inline void set_font_family(const std::string& new_family) { font_family_name = new_family; }

    /* Persistence */
    void save() const;  // Save configuration to file
    void load();        // Load configuration from file
  };

} // namespace Config_NS

#endif // GOSDCLOCK_CONFIG_HPP
