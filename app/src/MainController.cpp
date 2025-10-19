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

    engine::resources::Shader* MainController::create_model_shader() {
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto resources = get<engine::resources::ResourcesController>();

        // Only shader used for models, skybox has its own
        engine::resources::Shader *shader = resources->shader("model_shader");

        shader->use();

        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        shader->set_int("material.diffuse", material_diffuse);
        shader->set_int("material.specular", material_specular);
        shader->set_float("material.shininess", material_shininess);

        shader->set_vec3("dirLight.direction", dir_light_dir);
        shader->set_vec3("dirLight.ambient", dir_light_ambient);
        shader->set_vec3("dirLight.diffuse", dir_light_diffuse);
        shader->set_vec3("dirLight.specular", dir_light_specular);
        shader->set_float("dirLight.intensity", dir_light_intensity);


        shader->set_vec3("spotLight[0].direction", glm::vec3(0.91, -0.02f, -0.31f));
        shader->set_vec3("spotLight[0].position", glm::vec3(0.82f, 0.86f, -5.48f));
        shader->set_float("spotLight[0].cutOff", glm::cos(glm::radians(12.5f)));
        shader->set_float("spotLight[0].outerCutOff", glm::cos(glm::radians(17.5f)));
        shader->set_float("spotLight[0].intensity", spotlight_intensity);

        shader->set_vec3("spotLight[0].ambient", spotlight_ambient);
        shader->set_vec3("spotLight[0].diffuse", spotlight_diffuse);
        shader->set_vec3("spotLight[0].specular", spotlight_specular);

        shader->set_float("spotLight[0].constant", constant);
        shader->set_float("spotLight[0].linear", linear);
        shader->set_float("spotLight[0].quadratic", quadriatic);

        shader->set_vec3("spotLight[1].direction", glm::vec3(0.97f, -0.02f, -0.12f));
        shader->set_vec3("spotLight[1].position", glm::vec3(1.17f, 0.86f, -5.72f));
        shader->set_float("spotLight[1].cutOff", glm::cos(glm::radians(12.5f)));
        shader->set_float("spotLight[1].outerCutOff", glm::cos(glm::radians(17.5f)));
        shader->set_float("spotLight[1].intensity", spotlight_intensity);

        shader->set_vec3("spotLight[1].ambient", spotlight_ambient);
        shader->set_vec3("spotLight[1].diffuse", spotlight_diffuse);
        shader->set_vec3("spotLight[1].specular", spotlight_specular);

        shader->set_float("spotLight[1].constant", constant);
        shader->set_float("spotLight[1].linear", linear);
        shader->set_float("spotLight[1].quadratic", quadriatic);

        shader->set_bool("cameraLight.enabled", isCameraTorchOn);

        if (firstRun || isCameraTorchOn) {
            firstRun = false;

            auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
            shader->set_vec3("cameraLight.light.direction", camera->Front);
            shader->set_vec3("cameraLight.light.position", camera->Position);
            shader->set_float("cameraLight.light.cutOff", glm::cos(glm::radians(12.5f)));
            shader->set_float("cameraLight.light.outerCutOff", glm::cos(glm::radians(17.5f)));
            shader->set_float("cameraLight.light.intensity", camera_spotlight_intensity + 0.5f);

            shader->set_vec3("cameraLight.light.ambient", camera_spotlight_ambient);
            shader->set_vec3("cameraLight.light.diffuse", camera_spotlight_diffuse);
            shader->set_vec3("cameraLight.light.specular", camera_spotlight_specular);

            shader->set_float("cameraLight.light.constant", constant);
            shader->set_float("cameraLight.light.linear", linear);
            shader->set_float("cameraLight.light.quadratic", quadriatic);
        }

        return shader;
    }

    void MainController::draw() { draw_desert(); draw_ak47(); draw_heli(); draw_barn(); draw_jeep(); draw_skybox();}

    void MainController::draw_ak47() {
        auto resources = get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Shader* shader = create_model_shader();

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

        engine::resources::Shader* shader = create_model_shader();

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

        engine::resources::Shader* shader = create_model_shader();

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

        engine::resources::Shader* shader = create_model_shader();

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

        engine::resources::Shader* shader = create_model_shader();

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
        float movement_speed = dt * this->speed;

        if (platform->key(engine::platform::KEY_W).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::FORWARD, movement_speed); }

        if (platform->key(engine::platform::KEY_S).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, movement_speed); }

        if (platform->key(engine::platform::KEY_A).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::LEFT, movement_speed); }

        if (platform->key(engine::platform::KEY_D).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::RIGHT, movement_speed); }

        if (platform->key(engine::platform::KEY_SPACE).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::UP, movement_speed); }

        if (platform->key(engine::platform::KEY_LEFT_SHIFT).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::DOWN, movement_speed); }

        if (platform->key(engine::platform::KeyId::KEY_L).state() == engine::platform::Key::State::JustPressed) { isCameraTorchOn = !isCameraTorchOn; }

        auto mouse = platform->mouse();
        camera->rotate_camera(mouse.dx, mouse.dy);
        camera->zoom(mouse.scroll);
    }

}