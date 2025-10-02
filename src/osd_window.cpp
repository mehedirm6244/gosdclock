#include <gosdclock/osd_window.hpp>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>

OSD_Window::OSD_Window() {
  /* OSD Window Properties */

  // Enable RGBA visual for transparency support
  GdkScreen* screen = gdk_screen_get_default();
  if (auto visual = gdk_screen_get_rgba_visual(screen))
    gtk_widget_set_visual(GTK_WIDGET(this->gobj()), visual);

  set_title("gosdclock");
  set_size_request(-1, -1);           // Natural size
  set_border_width(0);
  set_decorated(false);
  set_resizable(false);
  set_keep_above(true);
  set_accept_focus(false);
  set_skip_taskbar_hint(true);
  set_skip_pager_hint(true);
  set_app_paintable(true);
  stick();                            // Stick to current workspace

  // Add CSS class for theming
  // https://docs.gtk.org/gtk3/css-overview.html
  get_style_context()->add_class("gosdclock");

  /* Add widgets */
  add(time_label);
  show_all_children();

  /* Apply settings after window allocation */
  signal_size_allocate().connect([this](Gtk::Allocation& alloc) {
    apply_settings();
  }, false);

  /* Update clock every 1 second */
  Glib::signal_timeout().connect(
    sigc::mem_fun(*this, &OSD_Window::update_time), 1000);

  /* Listen for hover events */
  add_events(Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
  signal_enter_notify_event().connect(
    sigc::mem_fun(*this, &OSD_Window::on_hover), false);
  signal_leave_notify_event().connect(
    sigc::mem_fun(*this, &OSD_Window::on_leave), false);
}

/* Update the time displayed */
bool OSD_Window::update_time() {
  std::time_t t = std::time(nullptr);
  // https://en.cppreference.com/w/cpp/chrono/c/tm.html
  std::tm tm = *std::localtime(&t);

  std::ostringstream oss_time;
  oss_time << std::put_time(&tm, config.get_time_format_pattern().c_str());
  time_label.set_text(oss_time.str());

  return true; // keep timeout running
}

/* Apply configuration settings */
void OSD_Window::apply_settings() {
  /* Font */
  Pango::FontDescription font_desc;
  font_desc.set_family(config.get_font_family());
  font_desc.set_size(config.get_font_size() * PANGO_SCALE);
  time_label.override_font(font_desc);

  /* Calculate window position */
  int pos_x = 0, pos_y = 0;
  int offset_x = config.get_offset_x();
  int offset_y = config.get_offset_y();

  auto screen_width = get_screen()->get_width();
  auto screen_height = get_screen()->get_height();
  auto win_width = get_width();
  auto win_height = get_height();

  switch (config.get_osd_position()) {
    case GOSDClock_Config::OSDPosition::TopLeft:
      pos_x = 0;
      pos_y = 0;
      break;
    case GOSDClock_Config::OSDPosition::TopRight:
      pos_x = screen_width - win_width;
      pos_y = 0;
      break;
    case GOSDClock_Config::OSDPosition::BottomLeft:
      pos_x = 0;
      pos_y = screen_height - win_height;
      break;
    case GOSDClock_Config::OSDPosition::BottomRight:
      pos_x = screen_width - win_width;
      pos_y = screen_height - win_height;
      break;
  }

  move(pos_x + offset_x, pos_y + offset_y);
}

/* Handle hover (mouse enter) */
bool OSD_Window::on_hover(GdkEventCrossing* event) {
  Glib::signal_timeout().connect_once([this]() {
    get_style_context()->add_class("gosdclock_hidden");
    set_keep_above(false);
    set_keep_below(true);
  }, 250); // delay before hiding

  return true; // stop further propagation
}

/* Handle leave (mouse exit) */
bool OSD_Window::on_leave(GdkEventCrossing* event) {
  Glib::signal_timeout().connect_once([this]() {
    get_style_context()->remove_class("gosdclock_hidden");
    set_keep_above(true);
    set_keep_below(false);
  }, 1000); // delay before showing

  return true; // stop further propagation
}
