#include <gosdclock/osd_window.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
  auto app = Gtk::Application::create(argc, argv, "org.gosdclock.app");
  OSD_Window osd_window;
  
  int status = app->run(osd_window);
  if (status) {
    std::cout << "gsdclock exited with code " << status << std::endl;
  }

  return status;
}
