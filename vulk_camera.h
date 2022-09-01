#pragma once


//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>


namespace vWind {

	class VulkCamera {
	public: 
		void setOrthoProjection(
			float left, float right, float top, float bottom, float near, float far);

		void setPerspectiveProjection(float fovy, float aspect, float near, float far);

		void setViewDirection(
			glm::vec3 postion,glm::vec3 direction, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
		void setViewTarget(
			glm::vec3 postion, glm::vec3 target, glm::vec3 up = glm::vec3{ 0.f, -1.f, 0.f });
		void setViewYXZ(glm::vec3 postion, glm::vec3 rotation);

		const glm::mat4 &getProjection() const { return projectionMat; }
		const glm::mat4& getView() const { return viewMat; }
	private:
		glm::mat4 projectionMat{ 1.f };
		glm::mat4 viewMat{ 1.f };


	};

}//namespace vWind