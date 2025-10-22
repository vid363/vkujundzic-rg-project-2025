#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP

#include "glm/ext/vector_common.hpp"


#include <engine/core/Controller.hpp>
#include <engine/resources/Shader.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

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
        float spotlight_intensity = 1.5f;

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

        // Material values
        int material_diffuse = 0;
        int material_specular = 1;
        float material_shininess = 128.0f;

        bool can_camera_go_below_ground = false;

        bool action_sequence = false;

        class Jeep {
            public:
                // TODO: move both light directions to jeep direction if time allows (hopefully)
                glm::vec3 light1_direction = glm::vec3(1.0f, -0.09f, -0.31);
                glm::vec3 light2_direction = glm::vec3(1.0f, -0.09f, -0.31);
                glm::vec3 light1_pos = glm::vec3(0.71f, 0.87f, -5.43f);
                glm::vec3 light2_pos = glm::vec3(0.98f, 0.87f, -4.65f);
                float rotation_z = 110.0f;
                glm::vec3 pos = glm::vec3(-2.0f, 0.6f, -4.0f);
        };

        class Helicopter {
            public:
                glm::vec3 position = glm::vec3(0.0f, 20.0f, -100.0f);
                glm::vec3 direction;
                float pitch = 50.0f;
                float yaw = 0.0f;
                float roll = 0.0f;
                float speed = 30.0f;
                float rotation_speed = 18.0f;
                float pitch_before_stabilizing = 30.0f;
                float yaw_before_stabilizing = 0.0f;
                float roll_before_stabilizing = 0.0f;
                bool reached_landing_dest = false;
                bool landing = false;
                bool stabilizing = false;
                int angle_sign = 1;
                bool switched_stabilization_direction = false;
                bool stabilized = false;
                bool landed = false;
                time_t time_landed = time(nullptr) + 99999;

                // Can be changed to just go into the direction but requires heli physics
                void move(float const dt, glm::vec3 dest, bool forward = false, bool up = false, bool side = false);

                void rotate(float const dt, float pitch = 0.0f, float yaw = 0.0f, float roll = 0.0f);

                void stabilize(float dt);

                void reset();
        };

        std::vector<glm::mat4> cactus_models;

        Helicopter helicopter;
        Jeep jeep_info;

    private:
        void initialize() override;

        bool loop() override;

        engine::resources::Shader* create_and_set_shader(glm::mat4* model, const std::string &shader_name);

        void begin_draw() override;

        void draw() override;

        void draw_ak47();

        void draw_heli();

        void draw_barn();

        void draw_desert();

        void draw_jeep();

        void draw_jeep_lights();

        void create_instance_models(uint32_t n);

        void draw_cactuses();

        void draw_skybox();

        void end_draw() override;

        void update() override;

        void update_sequence();

        void update_camera();

        public:
            std::string_view name() const override {
                return "app::MainController";
            }
    };
}

#endif
