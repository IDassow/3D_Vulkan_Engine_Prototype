#include "kb_movement_controller.h"
#include <limits>


namespace vWind {

	void KeyboardMovementController::moveInPlaneXZ(
		GLFWwindow* window, float dt, VulkGameObj &gameObject) {
	
		if (glfwGetKey(window, keys.exit) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		/*if (glfwRawMouseMotionSupported())
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		glfwGetCursorPos(window, &xpos, &ypos);
		
		float offset = (float)xpos - lastX;
		float offsety = (float)lastY - ypos;
		glm::vec3 rotateM{ 0 };
		if (glfwSetCursorPosCallback(window, cursor_position_callback) != NULL) { 
			rotateM.y += offset;
			rotateM.x += offsety;
		}
		if (glm::dot(rotateM, rotateM) > std::numeric_limits<float>::epsilon()) {
			gameObject.transform.rotation += (2*lookSpeed) * dt * glm::normalize(rotateM);
		}
		glfwSetCursorPosCallback(window, cursor_position_callback);


		void cursor_position_callback(GLFWwindow * window, double xpos, double ypos);*/

		//limit pitch values between about +/- '~85' degrees
		gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
		gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

		float yaw = gameObject.transform.rotation.y; //yaw: twist or oscillate about a vertical axis.
		const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
		const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
		const glm::vec3 upDir{ 0.f, -1.f, 0.f };

		glm::vec3 rotate{ 0 };
		if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.f;
		if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.f;
		if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.f;
		if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.f;
		
		if (glm::dot( rotate, rotate ) > std::numeric_limits<float>::epsilon()) {
			gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
		}


		glm::vec3 moveDir{ 0.f };
		if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
		if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS)moveDir -= forwardDir;
		if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS)	moveDir += rightDir;
		if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS)	moveDir -= rightDir;
		if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS)		moveDir += upDir;
		if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS)	moveDir -= upDir;

		if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
			gameObject.transform.translation += lookSpeed * dt * glm::normalize(moveDir);
		}


	}

	void KeyboardMovementController::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
		xpos = 800 / 2.;
		ypos = 600 / 2.;
		glfwSetCursorPos(window, xpos, ypos);
	}
	
}//namespace vWind
