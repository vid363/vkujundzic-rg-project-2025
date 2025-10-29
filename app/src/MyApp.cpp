#include <MyApp.hpp>
#include <spdlog/spdlog.h>
#include <MainController.hpp>
#include <GUIController.hpp>

namespace app {
    void MyApp::app_setup() {
        spdlog::info("App setup!");
        auto main_controller = register_controller<app::MainController>();
        auto gui_controller = register_controller<app::GUIController>();
        main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
        main_controller->before(gui_controller);
    }
}