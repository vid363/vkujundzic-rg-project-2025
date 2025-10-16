#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/platform/PlatformEventObserver.hpp"
#include "engine/resources/ResourcesController.hpp"
#include "spdlog/spdlog.h"

#include <MainController.hpp>

namespace app {
    void MainController::initialize() {
        spdlog::info("Initializing MainController...");
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

    void MainController::draw() { draw_ak47(); }

    void MainController::draw_ak47() {
        auto resources = get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Model* ak47 = resources->model("ak47");
        engine::resources::Shader *shader = resources->shader("basic");

        shader->use();

        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -15.0f));
        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        shader->set_mat4("model", model);

        ak47->draw(shader);
    }

    void MainController::end_draw() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->swap_buffers();
    }


}