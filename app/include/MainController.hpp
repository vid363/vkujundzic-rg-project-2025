#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP

#include <engine/core/Controller.hpp>
#include <glm/glm.hpp>
#include <engine/resources/Shader.hpp>

namespace app {
    class MainController final : public engine::core::Controller {

    public:
        float speed = 3.0f;

        // Light values
        glm::vec3 dir_light_dir = glm::vec3(1.0f, -1.0f, -1.0f);
        glm::vec3 dir_light_ambient = glm::vec3(0.2f, 0.2f, 0.2f);

        glm::vec3 dir_light_diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
        glm::vec3 dir_light_specular = glm::vec3(1.0f, 1.0f, 1.0f);
        float dir_light_intensity = 0.5f;

        // Spotlight values
        glm::vec3 spotlight_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
        glm::vec3 spotlight_diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
        glm::vec3 spotlight_specular = glm::vec3(1.0f, 1.0f, 1.0f);
        float spotlight_intensity = 1.0f;

        float constant = 1.0f;
        float linear = 0.09f;
        float quadriatic = 0.032f;


        // Camera spotlight values
        glm::vec3 camera_spotlight_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
        glm::vec3 camera_spotlight_diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
        glm::vec3 camera_spotlight_specular = glm::vec3(1.0f, 1.0f, 1.0f);
        float camera_spotlight_intensity = 1.0f;


        // Should camera spotlight be on
        bool isCameraTorchOn = false;
        bool firstRun = true;

        // Material valuesww
        int material_diffuse = 0;
        int material_specular = 1;
        float material_shininess = 128.0f;

        bool can_camera_go_below_ground = false;
    private:
        void initialize() override;

        bool loop() override;

        engine::resources::Shader* create_model_shader();

        void begin_draw() override;

        void draw() override;

        void draw_ak47();

        void draw_heli();

        void draw_barn();

        void draw_desert();

        void draw_jeep();

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
