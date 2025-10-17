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

    void MainController::draw() { draw_ak47(); draw_heli(); draw_barn(); draw_skybox();}

    void MainController::draw_ak47() {
        auto resources = get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Model* ak47 = resources->model("ak47");
        engine::resources::Shader *shader = resources->shader("basic");

        shader->use();

        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        shader->set_mat4("model", model);

        ak47->draw(shader);
    }

    void MainController::draw_heli() {
        auto resources = get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Model* heli = resources->model("heli");
        engine::resources::Shader *shader = resources->shader("basic");

        shader->use();

        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 6.0f, -5.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        shader->set_mat4("model", model);

        heli->draw(shader);
    }

    void MainController::draw_barn() {
        auto resources = get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Model* barn = resources->model("barn");
        engine::resources::Shader *shader = resources->shader("basic");

        shader->use();

        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.0f, -4.0f, -5.0f));
        model = glm::scale(model, glm::vec3(1.0f));
        shader->set_mat4("model", model);

        barn->draw(shader);
    }

    void MainController::draw_skybox() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
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