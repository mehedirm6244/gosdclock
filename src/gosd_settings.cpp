#include <gosdclock/settings_window.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
  auto app = Gtk::Application::create(argc, argv, "org.gosdclock_settings.app");
  SettingsWindow settings_window;
  
  int status = app->run(settings_window);
  if (status != 0) {
    std::cout << "gsdclock_settings exited with exit code " << status << std::endl;
  }

  return status;
}
