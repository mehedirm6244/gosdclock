#include <gosdclock/config.hpp>
#include <iostream>
#include <fstream>
#include <glibmm/main.h>
#include <glibmm/keyfile.h>

using namespace GOSDClock_Config;

/* Config class implementation */

Config::Config() {
  /* Set default configuration */
  font_size_px = 28;
  font_family_name = "sans";
  osd_position = OSDPosition::BottomRight;
  time_format_type = TimeFormat::TwelveHour;
  time_format_pattern = TIME_FORMAT_PATTERNS[static_cast<int>(time_format_type)];
  horizontal_offset = -15;
  vertical_offset = -15;

  /* Determine path to configuration file */
  config_file_path = std::filesystem::path(std::getenv("HOME")) / ".config/gosdclock/config.cfg";

  /* Create default config if file does not exist */
  if (!std::filesystem::exists(config_file_path)) {
    std::cerr << config_file_path << " does not exist. Generating default config.\n";
    std::filesystem::create_directories(config_file_path.parent_path());
    save();
  }

  /* Load existing configuration (or defaults if new) */
  load();

  /* Monitor config file for changes and reload automatically */
  auto last_write_time = std::filesystem::last_write_time(config_file_path);
  Glib::signal_timeout().connect_seconds([this, last_write_time]() mutable {
    auto current_write_time = std::filesystem::last_write_time(config_file_path);
    if (current_write_time != last_write_time) {
      last_write_time = current_write_time;
      std::cout << "Configuration changed, reloading...\n";
      load();
    }
    return true; // keep the timer running
  }, 2); // check every 2 seconds
}

/* Save current configuration to file */
void Config::save() const {
  Glib::KeyFile keyfile;

  /* Font settings */
  keyfile.set_string("Font", "font_family", font_family_name);
  keyfile.set_integer("Font", "font_size", font_size_px);

  /* OSD window settings */
  keyfile.set_integer("Window", "osd_position", static_cast<int>(osd_position));

  /* Window offset */
  keyfile.set_integer("Window", "offset_x", horizontal_offset);
  keyfile.set_integer("Window", "offset_y", vertical_offset);

  /* Time format */
  keyfile.set_integer("Window", "time_format", static_cast<int>(time_format_type));

  try {
    keyfile.save_to_file(config_file_path.string());
  } catch (const Glib::Error& e) {
    std::cerr << "Failed to save configuration: " << e.what() << "\n";
  }
}

/* Load configuration from file */
void Config::load() {
  Glib::KeyFile keyfile;
  try {
    keyfile.load_from_file(config_file_path.string());

    /* Font settings */
    if (keyfile.has_key("Font", "font_family"))
      font_family_name = keyfile.get_string("Font", "font_family");
    if (keyfile.has_key("Font", "font_size"))
      font_size_px = keyfile.get_integer("Font", "font_size");

    /* OSD window settings */
    if (keyfile.has_key("Window", "osd_position"))
      osd_position = static_cast<OSDPosition>(keyfile.get_integer("Window", "osd_position"));

    /* Window offset */
    if (keyfile.has_key("Window", "offset_x"))
      horizontal_offset = keyfile.get_integer("Window", "offset_x");
    if (keyfile.has_key("Window", "offset_y"))
      vertical_offset = keyfile.get_integer("Window", "offset_y");

    /* Time format */
    if (keyfile.has_key("Window", "time_format")) {
      int time_format = keyfile.get_integer("Window", "time_format");
      time_format_type = static_cast<TimeFormat>(time_format);
      time_format_pattern = TIME_FORMAT_PATTERNS[time_format];
    }

  } catch (const Glib::Error& e) {
    std::cerr << "Failed to load configuration: " << e.what() << "\n";
  }
}
