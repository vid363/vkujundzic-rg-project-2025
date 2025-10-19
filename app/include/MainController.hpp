#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP

#include <engine/core/Controller.hpp>
#include <glm/glm.hpp>
#include <engine/resources/Shader.hpp>

namespace app {
    class MainController final : public engine::core::Controller {
    public:
        float get_speed() const { return speed; }
    private:
        float speed = 3.0f;

        // Light values
        glm::vec3 light_dir = glm::vec3(1.0f, -1.0f, -1.0f);
        glm::vec3 light_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
        glm::vec3 light_diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
        glm::vec3 light_specular = glm::vec3(1.0f, 1.0f, 1.0f);

        // Material values
        int material_diffuse = 0;
        int material_specular = 1;
        float material_shininess = 64.0f;

        void initialize() override;

        bool loop() override;

        engine::resources::Shader* create_shader(const std::string& shader);

        void begin_draw() override;

        void draw() override;

        void draw_ak47();

        void draw_heli();

        void draw_barn();

        void draw_desert();

        void draw_truck();

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
