#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/application.h>

int main(int argc, char *argv[]){
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");
    Gtk::Window window;
    window.set_default_size(200, 200);
    return app->run(window);
}