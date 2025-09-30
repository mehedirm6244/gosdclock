#ifndef GOSDCLOCK_CONFIG_HPP
#define GOSDCLOCK_CONFIG_HPP

#include <string>
#include <filesystem>

namespace Config_NS {
  enum DISPLAY_POSITION {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
  };

  enum TIME_FORMAT {
    TWELVE_HR,
    TWELVE_HR_SEC,
    TWENTYFOUR_HR,
    TWENTYFOUR_HR_SEC,
  };

  class Config {
  private:
    std::filesystem::path m_config_path;

    /* Configuration */
    int offset_x; // (px)
    int offset_y; // (px)
    DISPLAY_POSITION osd_position;

    int font_size; // (px)
    std::string font_family;

    TIME_FORMAT time_format;
    std::string time_format_placeholder;

  public:
    Config();
    ~Config() = default;
    std::string get_config_path();

    /* Getter functions */
    inline int get_font_size() { return font_size; }
    inline int get_offset_x() { return offset_x; }
    inline int get_offset_y() { return offset_y; }
    inline DISPLAY_POSITION get_osd_position() { return osd_position; }
    inline TIME_FORMAT get_time_format() { return time_format; }
    inline std::string get_time_format_placeholder() { return time_format_placeholder; }
    inline std::string get_font_family() { return font_family; }

    /* Setter functions */
    inline void set_font_size(const int sz) { font_size = sz; }
    inline void set_offset_x(const int offset) { offset_x = offset; }
    inline void set_offset_y(const int offset) { offset_y = offset; }
    inline void set_osd_position(DISPLAY_POSITION pos) { osd_position = pos; }
    inline void set_time_format(TIME_FORMAT format) { time_format = format; }
    inline void set_time_format_placeholder(std::string& format) { time_format_placeholder = format; }
    inline void set_font_family(const std::string& name) { font_family = name; }

    void save() const;
    void load();
  };
}

#endif // OSDCLOCK_CONFIG_HPP
