#ifndef GUI_CONTROLLER_HPP
#define GUI_CONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace app {
    class GUIController final : public engine::core::Controller {
        public:
            std::string_view name() const override { return "app::GUIController"; }
        private:
            void initialize() override;

            void poll_events() override;

            void draw() override;
    };
}

#endif
