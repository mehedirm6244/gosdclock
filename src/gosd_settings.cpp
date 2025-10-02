#include <gosdclock/settings_window.hpp>
#include <iostream>

/**
 * @brief Entry point for GOSDClock Settings application.
 */
int main(int argc, char* argv[]) {
  auto app = Gtk::Application::create(argc, argv, "org.gosdclock.settings.app");
  SettingsWindow settings_window;

  int status = app->run(settings_window);

  if (status != 0)
    std::cerr << "GOSDClock Settings exited with code " << status << std::endl;

  return status;
}
