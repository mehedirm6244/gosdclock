#ifndef GOSDCLOCK_OSD_WINDOW_HPP
#define GOSDCLOCK_OSD_WINDOW_HPP

#include <gtkmm.h>
#include <gosdclock/config.hpp>
#include <gosdclock/settings_window.hpp>

/**
 * @brief On-Screen Display (OSD) window for the clock.
 * 
 * Displays the current time on top of other windows according to
 * user-configured settings.
 */
class OSD_Window : public Gtk::Window {
public:
  OSD_Window();
  ~OSD_Window() = default;

  /**
   * @brief Apply the current configuration settings to the OSD window.
   * Updates font, position, offsets, and time format.
   */
  void apply_settings();

private:
  /* Configuration */
  GOSDClock_Config::Config config;   // Stores user preferences (position, font, time format)

  /* Widgets */
  Gtk::Label time_label;      // Label that displays the current time

  /* Mouse event handlers */
  bool on_hover(GdkEventCrossing* event);
  bool on_leave(GdkEventCrossing* event);

  /* Helper Functions */
  bool update_time();         // Update the time displayed in the label
};

#endif // GOSDCLOCK_OSD_WINDOW_HPP
