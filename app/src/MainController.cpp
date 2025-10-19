#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/platform/PlatformEventObserver.hpp"
#include "engine/resources/ResourcesController.hpp"
#include "spdlog/spdlog.h"

#include <MainController.hpp>
#include <GUIController.hpp>

namespace app {

    // TODO: Maybe move to seperate
    class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
        public:
            void on_mouse_move(engine::platform::MousePosition position) override;
            void on_scroll(engine::platform::MousePosition position) override;
    };

    void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
        auto gui = engine::core::Controller::get<GUIController>();
        if (gui->is_enabled()) return;

        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        // Temporary solution
        auto sensitivity = 5.0f;
        camera->rotate_camera(position.dx * sensitivity, position.dy * sensitivity);
    }

    void MainPlatformEventObserver::on_scroll(engine::platform::MousePosition position) {
        auto gui = engine::core::Controller::get<GUIController>();
        if (gui->is_enabled()) return;

        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera = graphics->camera();
        camera->zoom(position.dy);
        graphics->perspective_params().FOV = glm::radians(camera->Zoom);

    }

    void MainController::initialize() {
        spdlog::info("Initializing MainController...");
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
        engine::graphics::OpenGL::enable_depth_testing();
    }

    bool MainController::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KEY_ESCAPE).is_down()) {
            spdlog::info("Exiting...");
            return false;
        }
        return true;
    }

    void MainController::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }

    engine::resources::Shader* MainController::create_shader(const std::string& shader_name) {
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto resources = get<engine::resources::ResourcesController>();

        engine::resources::Shader *shader = resources->shader(shader_name);

        shader->use();

        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());


        if (shader_name == "directional") {
            shader->set_int("material.diffuse", material_diffuse);
            shader->set_int("material.specular", material_specular);
            shader->set_float("material.shininess", material_shininess);

            shader->set_vec3("light.direction", light_dir);
            shader->set_vec3("light.ambient", light_ambient);
            shader->set_vec3("light.diffuse", light_diffuse);
            shader->set_vec3("light.specular", light_specular);
        }

        if (shader_name == "spotlight") {
            shader->set_int("material.diffuse", material_diffuse);
            shader->set_int("material.specular", material_specular);
            shader->set_float("material.shininess", material_shininess);

            auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
            // shader->set_vec3("light.direction", light_dir);
            shader->set_vec3("light.direction", camera->Front);
            // shader->set_vec3("light.position", glm::vec3(-1.0f, 2.0f, -1.0f));
            shader->set_vec3("light.position", camera->Position);
            shader->set_float("light.cutOff", glm::cos(glm::radians(12.5f)));
            shader->set_float("light.outerCutOff", glm::cos(glm::radians(17.5f)));

            shader->set_float("light.constant", 1.0f);
            shader->set_float("light.linear", 0.09f);
            shader->set_float("light.quadratic", 0.032f);

            shader->set_vec3("light.ambient", light_ambient);
            shader->set_vec3("light.diffuse", light_diffuse);
            shader->set_vec3("light.specular", light_specular);
        }

        return shader;
    }


    void MainController::draw() { draw_desert(); draw_ak47(); draw_heli(); draw_barn(); draw_jeep(); draw_skybox();}

    void MainController::draw_ak47() {
        auto resources = get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Shader* shader = create_shader( "directional");

        engine::resources::Model *ak47 = resources->model("ak47");
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.63f, 0.48f, -6.0f));
        model = glm::rotate(model, glm::radians(240.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.15f));

        shader->set_mat4("model", model);

        ak47->draw(shader);
    }

    void MainController::draw_heli() {
        auto resources = get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Shader* shader = create_shader( "directional");

        engine::resources::Model *heli = resources->model("heli");
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 6.0f, -5.0f));
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f ));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f ));
        model = glm::scale(model, glm::vec3(0.8f));
        shader->set_mat4("model", model);

        heli->draw(shader);
    }

    void MainController::draw_barn() {
        auto resources = get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Shader* shader = create_shader( "spotlight");

        engine::resources::Model *barn = resources->model("barn");
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.0f, 2.5f, -5.0f));
        // model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f ));
        model = glm::scale(model, glm::vec3(5.0f));
        shader->set_mat4("model", model);

        barn->draw(shader);
    }

    void MainController::draw_jeep() {
        auto resources = get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Shader* shader = create_shader( "directional");

        engine::resources::Model *jeep = resources->model("jeep");
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, 0.6f, -4.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f ));
        model = glm::rotate(model, glm::radians(110.0f), glm::vec3(0.0f, 0.0f, 1.0f ));
        model = glm::scale(model, glm::vec3(0.7f));
        shader->set_mat4("model", model);

        jeep->draw(shader);
    }


    void MainController::draw_desert() {
        auto resources = get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Shader* shader = create_shader( "directional");

        engine::resources::Model *desert = resources->model("desert");
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        shader->set_mat4("model", model);

        desert->draw(shader);
    }


    void MainController::draw_skybox() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

        // No need to use create_shader func for skybox
        auto skybox = resources->skybox("night_skybox");
        auto shader = resources->shader("skybox");
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        graphics->draw_skybox(shader, skybox);
    }

    void MainController::end_draw() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->swap_buffers();
    }

    void MainController::update() { update_camera(); }

    void MainController::update_camera() {
        auto gui = engine::core::Controller::get<GUIController>();
        if (gui->is_enabled()) return;

        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera = graphics->camera();

        float dt = platform->dt();
        float movement_speed = dt * this->get_speed();

        if (platform->key(engine::platform::KEY_W).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::FORWARD, movement_speed); }

        if (platform->key(engine::platform::KEY_S).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, movement_speed); }

        if (platform->key(engine::platform::KEY_A).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::LEFT, movement_speed); }

        if (platform->key(engine::platform::KEY_D).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::RIGHT, movement_speed); }

        if (platform->key(engine::platform::KEY_SPACE).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::UP, movement_speed); }

        if (platform->key(engine::platform::KEY_LEFT_SHIFT).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::DOWN, movement_speed); }

        auto mouse = platform->mouse();
        camera->rotate_camera(mouse.dx, mouse.dy);
        camera->zoom(mouse.scroll);
    }

}