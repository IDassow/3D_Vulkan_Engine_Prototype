#include "first_vulkan_app.h"
#include "simple_render_system.h"
#include "vulk_camera.h"
#include "kb_movement_controller.h"
#include "vulk_buffer.h"

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

//std
#include <array>
#include <chrono> //implement timing
#include <cassert>
#include <iostream>

namespace vWind {

    struct GlobalUbo {
         glm::mat4 projectionView{ 1.f };
         glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };  // w is intensity
         glm::vec3 lightPosition{ -1.f };
         
         alignas(16) glm::vec4 lightColor{ 1.f };  // w is light intensity
         
        //alignas(16) glm::vec3 pointLight;
    };

    FirstApp::FirstApp() 
    { 
        globalPool = VulkDescriptorPool::Builder(vulkDevice)
            .setMaxSets(VulkSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VulkSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();

        loadGameObjects(); 
    }

    FirstApp::~FirstApp() { }

    void FirstApp::run() {

        std::vector<std::unique_ptr<VulkBuffer>> uboBuffers(VulkSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++) {
            uboBuffers[i] = std::make_unique<VulkBuffer>(
                vulkDevice,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);//| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
            uboBuffers[i]->map();
        }

        auto globalSetLayout = VulkDescriptorSetLayout::Builder(vulkDevice)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
            .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(VulkSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            VulkDescriptorWriter(*globalSetLayout,*globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        SimpleRenderSystem simpleRenderSystem
        {   
            vulkDevice, 
            vulkRenderer.getSCRenderPass(), 
            globalSetLayout->getDescriptorSetLayout()
        };
        VulkCamera camera{};

        auto viewerObject = VulkGameObj::createGameObject();
        viewerObject.transform.translation.z = -2.5f;
        KeyboardMovementController cameraController{};
        
        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!vulkWindow.shouldClose()){
            glfwPollEvents();

            //calc deltatime
            auto newTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;
            
            cameraController.moveInPlaneXZ(vulkWindow.getGLFWwindow(), deltaTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);
            float aspect = vulkRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

           
            if (auto commandBuffer = vulkRenderer.beginFrame()) {
                int frameIndex = vulkRenderer.getFrameIndex();
                FrameInfo frameInfo
                { 
                    frameIndex, deltaTime, 
                    commandBuffer, camera, 
                    globalDescriptorSets[frameIndex]
                };

                //update
                GlobalUbo ubo{};
                ubo.projectionView = camera.getProjection() * camera.getView();
                uboBuffers[frameIndex]->writeToIndex(&ubo, frameIndex);
                uboBuffers[frameIndex]->flush();
               
                ///render
                vulkRenderer.beginSCRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
                vulkRenderer.endSCRenderPass(commandBuffer);
                vulkRenderer.endFrame();
            }
          
        }
        vkDeviceWaitIdle(vulkDevice.device());
    }
    
    void FirstApp::loadGameObjects() {
        std::shared_ptr<VulkModel> vulkModel = 
            VulkModel::createModelFromFile(vulkDevice, "models/donut.obj");
        auto gameObj = VulkGameObj::createGameObject();
        gameObj.model = vulkModel;
        gameObj.transform.translation = { -1.0f,  .5f ,.0f };
        gameObj.transform.scale = glm::vec3 { 3.f };
        gameObjects.push_back(std::move(gameObj));

        vulkModel =
            VulkModel::createModelFromFile(vulkDevice, "models/smooth_vase.obj");
        auto VaseObj = VulkGameObj::createGameObject();
        VaseObj.model = vulkModel;
        VaseObj.transform.translation = { 1.0f,  .5f , .0f };
        VaseObj.transform.scale = glm::vec3{ 3.f };
        gameObjects.push_back(std::move(VaseObj));

        vulkModel =
            VulkModel::createModelFromFile(vulkDevice, "models/quad.obj");
        auto ground = VulkGameObj::createGameObject();
        ground.model = vulkModel;
        ground.transform.translation = { .0f,  .65f , .0f };
        ground.transform.scale = glm::vec3{ 3.f, 1.f, 3.f };
        gameObjects.push_back(std::move(ground));

       
    }

}//endof namespace vWind