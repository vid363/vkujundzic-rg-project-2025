#include "MainController.hpp"
#include "imgui.h"
#include <GUIController.hpp>
#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>

namespace app {
    void GUIController::initialize() {
        set_enable(false);
    }

    void GUIController::poll_events() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_K).state() == engine::platform::Key::State::JustPressed) set_enable(!is_enabled());
    }

    void GUIController::draw() {
        auto mainController = engine::core::Controller::get<app::MainController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        graphics->begin_gui();
        const auto &c = *camera;

        ImGui::Begin("INFO");

        if (ImGui::BeginTabBar("TABS")) {
            if (ImGui::BeginTabItem("Camera")) {

                ImGui::Text("Camera position: (%f, %f, %f)", c.Position.x, c.Position.y, c.Position.z);
                ImGui::Text("(Yaw, Pitch): (%f, %f)", c.Yaw, c.Pitch);
                ImGui::Text("Camera front: (%f, %f, %f)", c.Front.x, c.Front.y, c.Front.z);

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Light")) {
                ImGui::DragFloat3("Directional light ambient color", &mainController->dir_light_ambient.x, 0.01, 0.0, 100.0);
                ImGui::DragFloat3("Directional light diffuse color", &mainController->dir_light_diffuse.x, 0.01, 0.0, 100.0);
                ImGui::DragFloat3("Directional light specular color", &mainController->dir_light_specular.x, 0.01, 0.0, 100.0);
                ImGui::DragFloat("Directional light intensity", &mainController->dir_light_intensity, 0.01, 0.0, 1.0);

                ImGui::DragFloat3("Camera spotlight ambient color", &mainController->camera_spotlight_ambient.x, 0.01, 0.0, 100.0);
                ImGui::DragFloat3("Camera spotlight diffuse color", &mainController->camera_spotlight_diffuse.x, 0.01, 0.0, 100.0);
                ImGui::DragFloat3("Camera spotlight specular color", &mainController->camera_spotlight_specular.x, 0.01, 0.0, 100.0);
                ImGui::DragFloat("Camera spotlight intensity", &mainController->camera_spotlight_intensity, 0.01, 0.0, 1.0);

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
        graphics->end_gui();
    }

}