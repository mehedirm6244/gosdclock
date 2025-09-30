#ifndef GOSDCLOCK_OSD_WINDOW_HPP
#define GOSDCLOCK_OSD_WINDOW_HPP

#include <gtkmm.h>
#include <gosdclock/config.hpp>
#include <gosdclock/settings_window.hpp>

class OSD_Window : public Gtk::Window {
public:
  OSD_Window();
  ~OSD_Window() = default;

  void apply_settings(); // Apply config

private:
  /* Configuration */
  Config_NS::Config m_config;
  std::filesystem::path m_config_path;

  /* Time widget */
  Gtk::Label m_label_time;

  /* Signal handlers */
  bool on_hover(GdkEventCrossing* event);
  bool on_leave(GdkEventCrossing* event);

  bool update_time();
};

#endif // GOSDCLOCK_OSD_WINDOW_HPP
