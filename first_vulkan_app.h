#pragma once

#include "vulk_window.h"

#include "vulk_device.h"
#include "vulk_renderer.h"
#include "vulk_game_object.h"
#include "vulk_descriptors.h"



//std
#include <memory>
#include <vector>
namespace vWind {

	class FirstApp {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();

	private:
		void loadGameObjects();
	
		//void sierpinski(std::vector<VulkModel::Vertex>& vertices, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top);

		VulkWindow vulkWindow{ WIDTH, HEIGHT, "Vulkan Window Running..." };
		VulkDevice vulkDevice{ vulkWindow };
		VulkRenderer vulkRenderer{ vulkWindow, vulkDevice };
		
		//NOTE: order of declarations matters
		std::unique_ptr<VulkDescriptorPool> globalPool{};
		std::vector<VulkGameObj> gameObjects;
	};
}//endof namespace vWind