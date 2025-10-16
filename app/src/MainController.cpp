#include "engine/platform/PlatformController.hpp"
#include "engine/platform/PlatformEventObserver.hpp"
#include "spdlog/spdlog.h"

#include <MainController.hpp>

namespace app {
    void MainController::initialize() {
        spdlog::info("Initializing...");
    }

    bool MainController::loop() {
        spdlog::info("Looping...");
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KEY_ESCAPE).is_down()) {
            spdlog::info("Exiting...");
            return false;
        }
        return true;
    }
}