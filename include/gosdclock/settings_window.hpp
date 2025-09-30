#ifndef GOSDCLOCK_SETTINGS_WINDOW_HPP
#define GOSDCLOCK_SETTINGS_WINDOW_HPP

#include <gtkmm.h>
#include <gosdclock/config.hpp>

class SettingsWindow : public Gtk::Window {
public:
  SettingsWindow();
  ~SettingsWindow() = default;

private:
  Config_NS::Config m_config;
  std::filesystem::path m_config_path;

  /* Widgets */

  /* Main container */
  Gtk::Box m_vbox_main{Gtk::Orientation::ORIENTATION_VERTICAL, 10};

  /* Font widgets */
  Gtk::Label m_label_font_family;
  Gtk::FontButton m_button_font;

  /* OSD position widgets */
  Gtk::Label m_label_position;
  Gtk::ComboBoxText m_combo_position;

  /* Time format widgets */
  Gtk::Label m_label_timeformat;
  Gtk::ComboBoxText m_combo_timeformat;

  /* Offset widgets */
  Gtk::Label m_label_offset_x;
  Gtk::Label m_label_offset_y;
  Gtk::SpinButton m_spinbutton_offset_x;
  Gtk::SpinButton m_spinbutton_offset_y;

  /* Actionbar and children */
  Gtk::ActionBar m_actionbar;
  Gtk::Button m_btn_save;
  Gtk::Button m_btn_apply;
  Gtk::Button m_btn_cancel;

  // Signals
  void on_save_clicked();
  void on_apply_clicked();
  void on_cancel_clicked();
};

#endif // GOSDCLOCK_SETTINGS_WINDOW_HPP
