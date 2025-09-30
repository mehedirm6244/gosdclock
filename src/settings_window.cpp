#include <gosdclock/settings_window.hpp>
#include <iostream>

SettingsWindow::SettingsWindow()
: m_label_font_family("Font Family"),
m_label_position("OSD Position"),
m_label_timeformat("Time Format"),
m_label_offset_x("Offset X"),
m_label_offset_y("Offset Y"),
m_btn_apply("Apply"),
m_btn_save("Save"),
m_btn_cancel("Cancel")
{
  /* Set Window Properties */
  set_title("GOSDClock Settings");
  set_default_size(300, 200);
  set_border_width(12);
  set_resizable(false);

  add(m_vbox_main);

  /* Font Style */
  m_button_font.set_font_name(m_config.get_font_family());
  m_button_font.set_show_size(false);
  m_vbox_main.pack_start(m_label_font_family, Gtk::PackOptions::PACK_SHRINK);
  m_vbox_main.pack_start(m_button_font, Gtk::PackOptions::PACK_SHRINK);

  /* OSD Position */
  m_combo_position.append("Top left");
  m_combo_position.append("Top right");
  m_combo_position.append("Bottom left");
  m_combo_position.append("Bottom right");

  // Set current position
  switch(m_config.get_osd_position()) {
    case Config_NS::DISPLAY_POSITION::TOP_LEFT:
      m_combo_position.set_active_text("Top left");
      break;

    case Config_NS::DISPLAY_POSITION::TOP_RIGHT:
      m_combo_position.set_active_text("Top right");
      break;

    case Config_NS::DISPLAY_POSITION::BOTTOM_LEFT:
      m_combo_position.set_active_text("Bottom left");
      break;

    case Config_NS::DISPLAY_POSITION::BOTTOM_RIGHT:
      m_combo_position.set_active_text("Bottom right");
      break;
  }

  m_vbox_main.pack_start(m_label_position, Gtk::PackOptions::PACK_SHRINK);
  m_vbox_main.pack_start(m_combo_position, Gtk::PackOptions::PACK_SHRINK);

  /* Time Format */
  m_combo_timeformat.append("12 Hour");
  m_combo_timeformat.append("12 Hour (Sec)");
  m_combo_timeformat.append("24 Hour");
  m_combo_timeformat.append("24 Hour (Sec)");

  // Set current time format
  switch(m_config.get_time_format()) {
    case Config_NS::TIME_FORMAT::TWELVE_HR:
      m_combo_timeformat.set_active_text("12 Hour");
      break;

    case Config_NS::TIME_FORMAT::TWELVE_HR_SEC:
      m_combo_timeformat.set_active_text("12 Hour (Sec)");
      break;

    case Config_NS::TIME_FORMAT::TWENTYFOUR_HR:
      m_combo_timeformat.set_active_text("24 Hour");
      break;

    case Config_NS::TIME_FORMAT::TWENTYFOUR_HR_SEC:
      m_combo_timeformat.set_active_text("24 Hour (Sec)");
      break;
  }

  m_vbox_main.pack_start(m_label_timeformat, Gtk::PackOptions::PACK_SHRINK);
  m_vbox_main.pack_start(m_combo_timeformat, Gtk::PackOptions::PACK_SHRINK);

  /* Offset */
  m_spinbutton_offset_x.set_range(-10000, 10000); // I need to sleep
  m_spinbutton_offset_x.set_increments(1, 1);
  m_spinbutton_offset_x.set_value(m_config.get_offset_x());
  m_vbox_main.pack_start(m_label_offset_x, Gtk::PackOptions::PACK_SHRINK);
  m_vbox_main.pack_start(m_spinbutton_offset_x, Gtk::PackOptions::PACK_SHRINK);

  m_spinbutton_offset_y.set_range(-10000, 10000);
  m_spinbutton_offset_y.set_increments(1, 1);
  m_spinbutton_offset_y.set_value(m_config.get_offset_y());
  m_vbox_main.pack_start(m_label_offset_y, Gtk::PackOptions::PACK_SHRINK);
  m_vbox_main.pack_start(m_spinbutton_offset_y, Gtk::PackOptions::PACK_SHRINK);

  /* Buttons */
  m_actionbar.pack_end(m_btn_save);
  m_actionbar.pack_end(m_btn_cancel);
  m_actionbar.pack_start(m_btn_apply);

  m_vbox_main.pack_start(m_actionbar, Gtk::PackOptions::PACK_SHRINK);

  show_all_children();

  /* Connect signals */
  m_btn_save.signal_clicked().connect(
    sigc::mem_fun(*this, &SettingsWindow::on_save_clicked));
  m_btn_apply.signal_clicked().connect(
    sigc::mem_fun(*this, &SettingsWindow::on_apply_clicked));
  m_btn_cancel.signal_clicked().connect(
    sigc::mem_fun(*this, &SettingsWindow::on_cancel_clicked));
}

void SettingsWindow::on_apply_clicked() {
  auto font_desc = m_button_font.get_font_name(); // e.g. "Monospace Bold 42"
  Pango::FontDescription pango_font(font_desc);

  m_config.set_font_family(pango_font.get_family());
  m_config.set_font_size(pango_font.get_size() / Pango::SCALE); // Pango units → px

  std::string pos_text = m_combo_position.get_active_text();
  if (pos_text == "Top left")
    m_config.set_osd_position(Config_NS::TOP_LEFT);
  else if (pos_text == "Top right")
    m_config.set_osd_position(Config_NS::TOP_RIGHT);
  else if (pos_text == "Bottom left")
    m_config.set_osd_position(Config_NS::BOTTOM_LEFT);
  else if (pos_text == "Bottom right")
    m_config.set_osd_position(Config_NS::BOTTOM_RIGHT);
  
  std::string time_format_str = m_combo_timeformat.get_active_text();
  if (time_format_str == "12 Hour")
    m_config.set_time_format(Config_NS::TWELVE_HR);
  if (time_format_str == "12 Hour (Sec)")
    m_config.set_time_format(Config_NS::TWELVE_HR_SEC);
  if (time_format_str == "24 Hour")
    m_config.set_time_format(Config_NS::TWENTYFOUR_HR);
  if (time_format_str == "24 Hour (Sec)")
    m_config.set_time_format(Config_NS::TWENTYFOUR_HR_SEC);

  m_config.set_offset_x(m_spinbutton_offset_x.get_value());
  m_config.set_offset_y(m_spinbutton_offset_y.get_value());

  m_config.save();
}

void SettingsWindow::on_save_clicked() {
  on_apply_clicked();
  hide();
}

void SettingsWindow::on_cancel_clicked() {
  hide();
}
