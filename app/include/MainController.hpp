#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP

#include <engine/core/Controller.hpp>

namespace app {
    class MainController : public engine::core::Controller {
        void initialize() override;

        bool loop() override;

        void begin_draw() override;

        void draw() override;

        void draw_ak47();

        void end_draw() override;

        public:
            std::string_view name() const override {
                return "app::MainController";
            }
    };
}

#endif
