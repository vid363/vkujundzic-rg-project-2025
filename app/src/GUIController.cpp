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

                ImGui::Checkbox("Can camera go underground", &mainController->can_camera_go_below_ground);

                ImGui::DragFloat3("Camera spotlight ambient color", &mainController->camera_spotlight_ambient.x, 0.01, 0.0, 100.0);
                ImGui::DragFloat3("Camera spotlight diffuse color", &mainController->camera_spotlight_diffuse.x, 0.01, 0.0, 100.0);
                ImGui::DragFloat3("Camera spotlight specular color", &mainController->camera_spotlight_specular.x, 0.01, 0.0, 100.0);
                ImGui::DragFloat("Camera spotlight intensity", &mainController->camera_spotlight_intensity, 0.01, 0.0, 1.0);

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Directional light")) {
                ImGui::DragFloat3("Directional light ambient color", &mainController->dir_light_ambient.x, 0.01, 0.0, 100.0);
                ImGui::DragFloat3("Directional light diffuse color", &mainController->dir_light_diffuse.x, 0.01, 0.0, 100.0);
                ImGui::DragFloat3("Directional light specular color", &mainController->dir_light_specular.x, 0.01, 0.0, 100.0);
                ImGui::DragFloat("Directional light intensity", &mainController->dir_light_intensity, 0.01, 0.0, 1.0);

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Helicopter")) {
                if (mainController->action_sequence) {
                    ImGui::Text("Heli position: (%f, %f, %f)", mainController->helicopter.position.x, mainController->helicopter.position.y, mainController->helicopter.position.z);
                    ImGui::Text("Heli direction: (%f, %f, %f))", mainController->helicopter.direction.x, mainController->helicopter.direction.y, mainController->helicopter.direction.z);
                    ImGui::Text("Heli pitch: %f", mainController->helicopter.pitch);
                    ImGui::Text("Heli yawn: %f", mainController->helicopter.yaw);
                    ImGui::Text("Heli roll: %f", mainController->helicopter.roll);
                } else {
                    ImGui::Text("No helicopter in sight");
                }
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
        graphics->end_gui();
    }

}