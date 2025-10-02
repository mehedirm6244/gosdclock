#include <gosdclock/settings_window.hpp>
#include <iostream>

SettingsWindow::SettingsWindow()
: font_label("Font Family"),
  position_label("OSD Position"),
  time_format_label("Time Format"),
  offset_x_label("Offset X"),
  offset_y_label("Offset Y"),
  apply_button("Apply"),
  save_button("Save"),
  cancel_button("Cancel")
{
  /* Window Properties */
  set_title("GOSDClock Settings");
  set_default_size(300, 200);
  set_border_width(12);

  /* Font Style */
  std::string raw_font_name = config.get_font_family() + " " + std::to_string(config.get_font_size());
  font_button.set_font_name(raw_font_name);

  main_layout.pack_start(font_label, Gtk::PackOptions::PACK_SHRINK);
  main_layout.pack_start(font_button, Gtk::PackOptions::PACK_SHRINK);

  /* OSD Position */
  for (int i = 0; i < static_cast<int>(GOSDClock_Config::OSDPosition::Count); ++i)
    position_combo.append(GOSDClock_Config::OSD_POSITION_LABELS[i]);

  position_combo.set_active_text(
    GOSDClock_Config::OSD_POSITION_LABELS[static_cast<int>(config.get_osd_position())]);

  main_layout.pack_start(position_label, Gtk::PackOptions::PACK_SHRINK);
  main_layout.pack_start(position_combo, Gtk::PackOptions::PACK_SHRINK);

  /* Time Format */
  for (int i = 0; i < static_cast<int>(GOSDClock_Config::TimeFormat::Count); ++i)
    time_format_combo.append(GOSDClock_Config::TIME_FORMAT_LABELS[i]);

  time_format_combo.set_active_text(
    GOSDClock_Config::TIME_FORMAT_LABELS[static_cast<int>(config.get_time_format())]);

  main_layout.pack_start(time_format_label, Gtk::PackOptions::PACK_SHRINK);
  main_layout.pack_start(time_format_combo, Gtk::PackOptions::PACK_SHRINK);

  /* Offset */
  offset_x_spin.set_range(-10000, 10000);
  offset_x_spin.set_increments(1, 1);
  offset_x_spin.set_value(config.get_offset_x());
  main_layout.pack_start(offset_x_label, Gtk::PackOptions::PACK_SHRINK);
  main_layout.pack_start(offset_x_spin, Gtk::PackOptions::PACK_SHRINK);

  offset_y_spin.set_range(-10000, 10000);
  offset_y_spin.set_increments(1, 1);
  offset_y_spin.set_value(config.get_offset_y());
  main_layout.pack_start(offset_y_label, Gtk::PackOptions::PACK_SHRINK);
  main_layout.pack_start(offset_y_spin, Gtk::PackOptions::PACK_SHRINK);

  /* Action Buttons */
  action_bar.pack_end(save_button);
  action_bar.pack_end(cancel_button);
  action_bar.pack_start(apply_button);

  main_layout.pack_start(action_bar, Gtk::PackOptions::PACK_SHRINK);

  /* Show all widgets */
  add(main_layout);
  show_all_children();

  /* Connect signals */
  save_button.signal_clicked().connect(
    sigc::mem_fun(*this, &SettingsWindow::on_save_clicked));
  apply_button.signal_clicked().connect(
    sigc::mem_fun(*this, &SettingsWindow::on_apply_clicked));
  cancel_button.signal_clicked().connect(
    sigc::mem_fun(*this, &SettingsWindow::on_cancel_clicked));
}

/* Apply configuration without closing */
void SettingsWindow::on_apply_clicked() {
  /* Font */
  std::string font_desc = font_button.get_font_name();
  Pango::FontDescription pango_font(font_desc);

  config.set_font_family(pango_font.get_family());
  config.set_font_size(pango_font.get_size() / Pango::SCALE); // Pango units → px

  /* OSD Position */
  std::string pos_text = position_combo.get_active_text();
  for (int i = 0; i < static_cast<int>(GOSDClock_Config::OSDPosition::Count); ++i)
    if (pos_text == GOSDClock_Config::OSD_POSITION_LABELS[i]) {
      config.set_osd_position(static_cast<GOSDClock_Config::OSDPosition>(i));
      break;
    }

  /* Time Format */
  std::string time_text = time_format_combo.get_active_text();
  for (int i = 0; i < static_cast<int>(GOSDClock_Config::TimeFormat::Count); ++i)
    if (time_text == GOSDClock_Config::TIME_FORMAT_LABELS[i]) {
      config.set_time_format(static_cast<GOSDClock_Config::TimeFormat>(i));
      break;
    }

  /* Offsets */
  config.set_offset_x(offset_x_spin.get_value());
  config.set_offset_y(offset_y_spin.get_value());

  /* Save changes to file */
  config.save();
}

/* Save configuration and close window */
void SettingsWindow::on_save_clicked() {
  on_apply_clicked();
  hide();
}

/* Discard changes and close window */
void SettingsWindow::on_cancel_clicked() {
  hide();
}
