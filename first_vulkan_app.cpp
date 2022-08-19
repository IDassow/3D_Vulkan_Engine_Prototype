#include "first_vulkan_app.h"
#include "simple_render_system.h"

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

//std
#include <stdexcept>
#include <array>
#include <cassert>
#include <iostream>

namespace vWind {

    FirstApp::FirstApp() { loadGameObjects(); }

    FirstApp::~FirstApp() {}

    void FirstApp::run() {
        SimpleRenderSystem simpleRenderSystem{ vulkDevice, vulkRenderer.getSCRenderPass() };

        while (!vulkWindow.shouldClose())
        {
            glfwPollEvents();
            if (auto commandBuffer = vulkRenderer.beginFrame()) {

                /// <summary>
                /// Begin offscreen shadow pass
                /// render shadow casting objects
                /// end off
                /// </summary>

                vulkRenderer.beginSCRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                vulkRenderer.endSCRenderPass(commandBuffer);
                vulkRenderer.endFrame();
            }
          
        }
        vkDeviceWaitIdle(vulkDevice.device());
    }
    /*
    void FirstApp::sierpinski(
        std::vector<VulkModel::Vertex>& vertices,
        int depth,
        glm::vec2 left,
        glm::vec2 right,
        glm::vec2 top) {
        if (depth <= 0) {
            vertices.push_back({ top });
            vertices.push_back({ right });
            vertices.push_back({ left });
        }
        else {
            auto leftTop = 0.5f * (left + top);
            auto rightTop = 0.5f * (right + top);
            auto leftRight = 0.5f * (left + right);
            sierpinski(vertices, depth - 1, left, leftRight, leftTop);
            sierpinski(vertices, depth - 1, leftRight, right, rightTop);
            sierpinski(vertices, depth - 1, leftTop, rightTop, top);
        }
    }

    //sierpinski(vertices, 3, { -0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.0f, -0.5f });

    */
    std::unique_ptr<VulkModel> createCubeModel(VulkDevice& device, glm::vec3 offset) {
        std::vector<VulkModel::Vertex> vertices{

            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

            // top face (purple, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .0f, .8f}},
            {{.5f, -.5f, .5f}, {.9f, .0f, .8f}},
            {{-.5f, -.5f, .5f}, {.9f, .0f, .8f}},
            {{-.5f, -.5f, -.5f}, {.9f, .0f, .8f}},
            {{.5f, -.5f, -.5f}, {.9f, .0f, .8f}},
            {{.5f, -.5f, .5f}, {.9f, .0f, .8f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.9f, .0f, .0f}},
            {{.5f, .5f, .5f}, {.9f, .0f, .0f}},
            {{-.5f, .5f, .5f}, {.9f, .0f, .0f}},
            {{-.5f, .5f, -.5f}, {.9f, .0f, .0f}},
            {{.5f, .5f, -.5f}, {.9f, .0f, .0f}},
            {{.5f, .5f, .5f}, {.9f, .0f, .0f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.0f, .1f, .9f}},
            {{.5f, .5f, 0.5f}, {.0f, .1f, .9f}},
            {{-.5f, .5f, 0.5f}, {.0f, .1f, .9f}},
            {{-.5f, -.5f, 0.5f}, {.0f, .1f, .9f}},
            {{.5f, -.5f, 0.5f}, {.0f, .1f, .9f}},
            {{.5f, .5f, 0.5f}, {.0f, .1f, .9f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .9f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .9f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .9f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .9f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .9f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .9f, .1f}},

        };
        for (auto& v : vertices) {
            v.position += offset;
        }
        return std::make_unique<VulkModel>(device, vertices);
    }

    void FirstApp::loadGameObjects() {
        std::shared_ptr<VulkModel> vulkModel = createCubeModel(vulkDevice, { .0f, .0f, .0f });

        auto cube = VulkGameObj::createGameObject();
        cube.model = vulkModel;
        cube.transform.translation = { .0f,  .0f , .5f };
        cube.transform.scale = { .5f,  .5f , .5f };
        gameObjects.push_back(std::move(cube));
    }


}//endof namespace vWind