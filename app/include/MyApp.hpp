#ifndef MY_APP_HPP
#define MY_APP_HPP

#include <engine/core/App.hpp>

namespace app {
    class MyApp : public engine::core::App {
        void app_setup() override;
    };
}

#endif