#include <gosdclock/osd_window.hpp>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>

OSD_Window::OSD_Window() {
  /* OSD Window Properties */

  /* Add support for transparency */
  GdkScreen* screen = gdk_screen_get_default();
  GdkVisual* visual = gdk_screen_get_rgba_visual(screen);
  if (visual)
    gtk_widget_set_visual(GTK_WIDGET(this->gobj()), visual); // দুষ?টু ছেলে

  set_title("gosdclock");
  set_size_request(-1, -1);
  set_border_width(0);
  set_decorated(false);
  set_resizable(false);
  set_keep_above(true);
  set_accept_focus(false);
  set_skip_taskbar_hint(true);
  set_skip_pager_hint(true);
  set_app_paintable(true);
  stick();

  /* CSS class to make OSD themable: https://docs.gtk.org/gtk3/css-overview.html */
  get_style_context()->add_class("gosdclock");

  /* Add widgets */
  add(m_label_time);
  show_all_children();

  /*
    Apply settings once window knows it's actual dimension (height x width)
    This is to prevent miscalculating OSD location (posX, posY) based on
    its size
   */
  signal_size_allocate().connect([this](Gtk::Allocation& alloc) {
    apply_settings();
  }, false);

  /* Update clock every 1s */
  Glib::signal_timeout().connect(
    sigc::mem_fun(*this, &OSD_Window::update_time), 1000);

  /* Listen to events on window enter/leave */
  /* I don't know what line 51 does */
  add_events(Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
  signal_enter_notify_event().connect(
    sigc::mem_fun(*this, &OSD_Window::on_hover), false);
  signal_leave_notify_event().connect(
    sigc::mem_fun(*this, &OSD_Window::on_leave), false);
}

bool OSD_Window::update_time() {
  std::time_t t = std::time(nullptr);
  // Read https://en.cppreference.com/w/cpp/chrono/c/tm.html if confused
  std::tm tm = *std::localtime(&t);

  std::ostringstream oss_time;
  std::string time_format;

  oss_time << std::put_time(&tm, m_config.get_time_format_placeholder().c_str());
  m_label_time.set_text(oss_time.str());

  return true;
}

void OSD_Window::apply_settings() {
  /* Font */
  auto font_desc = Pango::FontDescription();
  font_desc.set_family(m_config.get_font_family());
  font_desc.set_size(m_config.get_font_size() * PANGO_SCALE);
  m_label_time.override_font(font_desc);

  /* OSD Position */
  int posX, posY;
  int offset_x = m_config.get_offset_x();
  int offset_y = m_config.get_offset_y();

  switch (m_config.get_osd_position()) {
    case Config_NS::DISPLAY_POSITION::TOP_LEFT:
      posX = 0,
      posY = 0;
      break;

    case Config_NS::DISPLAY_POSITION::TOP_RIGHT:
      posX = get_screen()->get_width() - get_width(),
      posY = 0;
      break;

    case Config_NS::DISPLAY_POSITION::BOTTOM_LEFT:
      posX = 0,
      posY = get_screen()->get_height() - get_height();
      break;

    case Config_NS::DISPLAY_POSITION::BOTTOM_RIGHT:
      posX = get_screen()->get_width() - get_width(),
      posY = get_screen()->get_height() - get_height();
      break;
  }

  move(posX + offset_x, posY + offset_y);
}

bool OSD_Window::on_hover(GdkEventCrossing* event) {
  Glib::signal_timeout().connect_once([this]() {
    get_style_context()->add_class("gosdclock_hidden");
    set_keep_above(false);
    set_keep_below(true);
  }, 250);

  return true;
}

bool OSD_Window::on_leave(GdkEventCrossing* event) {
  Glib::signal_timeout().connect_once([this]() {
    get_style_context()->remove_class("gosdclock_hidden");
    set_keep_above(true);
    set_keep_below(false);
  }, 1000);

  return true;
}
