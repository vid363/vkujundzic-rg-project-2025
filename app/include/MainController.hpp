#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP

#include <engine/core/Controller.hpp>

namespace app {
    class MainController final : public engine::core::Controller {
    public:
        float get_speed() const { return speed; }
    private:
        float speed = 3.0f;

        void initialize() override;

        bool loop() override;

        void begin_draw() override;

        void draw() override;

        void draw_ak47();

        void draw_heli();

        void draw_bmp();

        void draw_skybox();

        void end_draw() override;

        void update() override;

        void update_camera();

        public:
            std::string_view name() const override {
                return "app::MainController";
            }
    };
}

#endif
