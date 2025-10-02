#ifndef GOSDCLOCK_SETTINGS_WINDOW_HPP
#define GOSDCLOCK_SETTINGS_WINDOW_HPP

#include <gtkmm.h>
#include <gosdclock/config.hpp>

/**
 * @class SettingsWindow
 * @brief A window that allows the user to configure gosdclock settings,
 *        such as font, OSD position, time format, and offsets.
 */
class SettingsWindow : public Gtk::Window {
public:
  SettingsWindow();
  ~SettingsWindow() = default;

private:
  /* Configuration */
  GOSDClock_Config::Config config;          // Local copy of configuration

  /* Layout Containers */
  Gtk::Box main_layout{Gtk::Orientation::ORIENTATION_VERTICAL, 10};

  /* Font Selection Widgets */
  Gtk::Label font_label;
  Gtk::FontButton font_button;

  /* OSD Position Widgets */
  Gtk::Label position_label;
  Gtk::ComboBoxText position_combo;

  /* Time Format Widgets */
  Gtk::Label time_format_label;
  Gtk::ComboBoxText time_format_combo;

  /* Offset Widgets */
  Gtk::Label offset_x_label;
  Gtk::Label offset_y_label;
  Gtk::SpinButton offset_x_spin;
  Gtk::SpinButton offset_y_spin;

  /* Action Bar (Buttons) */
  Gtk::ActionBar action_bar;
  Gtk::Button save_button;
  Gtk::Button apply_button;
  Gtk::Button cancel_button;

  /* Signal Handlers */
  void on_save_clicked();
  void on_apply_clicked();
  void on_cancel_clicked();
};

#endif // GOSDCLOCK_SETTINGS_WINDOW_HPP
