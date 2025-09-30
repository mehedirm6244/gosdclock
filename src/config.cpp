#include <gosdclock/config.hpp>
#include <iostream>
#include <fstream>
#include <glibmm/main.h>
#include <glibmm/keyfile.h>

using namespace Config_NS;

Config::Config() {
  /* Set default config */
  font_size = 28;
  font_family = "sans";
  osd_position = BOTTOM_RIGHT;
  time_format = TWELVE_HR;
  time_format_placeholder = "%I:%M %p";
  offset_x = -10;
  offset_y = -10;

  /* Path to configuration file */
  m_config_path = std::filesystem::path(std::getenv("HOME"))  / ".config/gosdclock/config.cfg";

  /* Create config with default options if absent */
  if (!std::filesystem::exists(m_config_path)) {
    std::cerr << m_config_path << " does not exist. Generating default config.\n";
    std::filesystem::create_directories(m_config_path.parent_path());
    save();
  }

  load();

  /* Monitor config for changes and reload if necessary */
  auto last_write = std::filesystem::last_write_time(m_config_path);
  Glib::signal_timeout().connect_seconds([this, last_write]() mutable {
    auto current_write = std::filesystem::last_write_time(m_config_path);
    if (current_write != last_write) {
      last_write = current_write;
      std::cout << "Config changed, reloading...\n";
      load();
    }

    return true; // keep timer ticking
  }, 2); // check every 2 seconds
}

std::string Config::get_config_path() {
  return m_config_path;
}

/* Save current settings to config */
void Config::save() const {
  Glib::KeyFile keyfile;

  /* Font settings */
  keyfile.set_string("Font", "font_family", font_family);
  keyfile.set_integer("Font", "font_size", font_size);

  /* OSD window settings */
  std::string osd_pos;
  switch (osd_position) {
    case TOP_LEFT:
      osd_pos = "TOP_LEFT";
      break;
    case TOP_RIGHT:
      osd_pos = "TOP_RIGHT";
      break;
    case BOTTOM_LEFT:
      osd_pos = "BOTTOM_LEFT";
      break;
    case BOTTOM_RIGHT:
      osd_pos = "BOTTOM_RIGHT";
      break;
  }
  keyfile.set_string("Window", "osd_position", osd_pos);

  /* Window offset */
  keyfile.set_integer("Window", "offset_x", offset_x);
  keyfile.set_integer("Window", "offset_y", offset_y);

  /* Time format */
  std::string time_format_str;
  switch (time_format) {
    case TWELVE_HR:
      time_format_str = "TWELVE_HR";
      break;
    case TWELVE_HR_SEC:
      time_format_str = "TWELVE_HR_SEC";
      break;

    case TWENTYFOUR_HR:
      time_format_str = "TWENTYFOUR_HR";
      break;

    case TWENTYFOUR_HR_SEC:
      time_format_str = "TWENTYFOUR_HR_SEC";
      break;
  }
  keyfile.set_string("Window", "time_format", time_format_str);

  try {
    keyfile.save_to_file(m_config_path.string());
  } catch (const Glib::Error& e) {
    std::cerr << "Failed to save config: " << e.what() << "\n";
  }
}

void Config::load() {
  Glib::KeyFile keyfile;
  try {
    keyfile.load_from_file(m_config_path.string());

    /* Font settings */
    if (keyfile.has_key("Font", "font_family"))
      font_family = keyfile.get_string("Font", "font_family");
    if (keyfile.has_key("Font", "font_size"))
      font_size = keyfile.get_integer("Font", "font_size");

    /* OSD window settings */
    if (keyfile.has_key("Window", "osd_position")) {
      std::string pos = keyfile.get_string("Window", "osd_position");
      
      if (pos == "TOP_LEFT")
        osd_position = TOP_LEFT;
      else if (pos == "TOP_RIGHT")
        osd_position = TOP_RIGHT;
      else if (pos == "BOTTOM_LEFT")
        osd_position = BOTTOM_LEFT;
      else if (pos == "BOTTOM_RIGHT")
        osd_position = BOTTOM_RIGHT;
    }

    /* Window offset */
    if (keyfile.has_key("Window", "offset_x"))
      offset_x = keyfile.get_integer("Window", "offset_x");
    if (keyfile.has_key("Window", "offset_y"))
      offset_y = keyfile.get_integer("Window", "offset_y");

    /* Time format */
    if(keyfile.has_key("Window", "time_format")) {
      std::string time_format_str = keyfile.get_string("Window", "time_format");

      if (time_format_str == "TWELVE_HR") {
        time_format = TWELVE_HR;
        time_format_placeholder = "%I:%M %p";
      } else if (time_format_str == "TWELVE_HR_SEC") {
        time_format = TWELVE_HR_SEC;
        time_format_placeholder = "%I:%M %p %S";
      } else if (time_format_str == "TWENTYFOUR_HR") {
        time_format = TWENTYFOUR_HR;
        time_format_placeholder = "%H:%M";
      } else if (time_format_str == "TWENTYFOUR_HR_SEC") {
        time_format = TWENTYFOUR_HR_SEC;
        time_format_placeholder = "%H:%M %S";
      }
    }

  } catch (const Glib::Error& e) {
    std::cerr << "Failed to load config: " << e.what() << "\n";
  }
}
