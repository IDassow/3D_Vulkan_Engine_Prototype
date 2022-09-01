#pragma once

#include "vulk_game_object.h"
#include "vulk_window.h"

namespace vWind {
	class KeyboardMovementController {

	public:
        struct KeyMappings {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_E;
            int moveDown = GLFW_KEY_Q;
            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;
            int exit = GLFW_KEY_ESCAPE;
        };

        //mouse state
        bool firstMouse = true;
        float lastX = 800/2.f;
        float lastY = 600/2.f;
        glm::vec3 rotateM{ 0 };
        // position
        glm::vec3 position = glm::vec3(0, 0, 5);
        // horizontal angle : toward -Z
        float horizontalAngle = 3.14f;
        // vertical angle : 0, look at the horizon
        float verticalAngle = 0.0f;
        // Initial Field of View
        float initialFoV = 45.0f;

        double xpos, ypos;
    
        void moveInPlaneXZ(GLFWwindow* window, float dt, VulkGameObj &gameObject);
        static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);


        KeyMappings keys{};
        float moveSpeed{ 3.f };
        float lookSpeed{ 1.5f };

        
	};
}
