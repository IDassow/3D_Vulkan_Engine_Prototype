#pragma once

#include "vulk_model.h"

#include <glm/gtc/matrix_transform.hpp>

//std
#include <memory>

namespace vWind {

	struct TransformComponent {
		glm::vec3 translation{}; //offset
		glm::vec3 scale{ 1.f, 1.f, 1.f };
		glm::vec3 rotation{};

		glm::mat4 mat4() {
			auto transform = glm::translate(glm::mat4{ 1.f }, translation);
			
			transform = glm::rotate(transform, rotation.x, { 1.f, 0.f, 0.f });
			transform = glm::rotate(transform, rotation.y, { 0.f, 1.f, 0.f});
			transform = glm::rotate(transform, rotation.z, { 0.f, 0.f, 1.f });

			transform = glm::scale(transform, scale);
			
			return transform;
		}	
	};



	class VulkGameObj {
	public:
		using id_t = unsigned int;

		VulkGameObj(const VulkGameObj&) = delete;
		VulkGameObj& operator=(const VulkGameObj&) = delete;
		VulkGameObj(VulkGameObj&&) = default;
		VulkGameObj& operator=(VulkGameObj&&) = default;


		static VulkGameObj createGameObject() {
			static id_t currentId = 0;
			return VulkGameObj{ currentId++ };
		};

		id_t getId() { return id; }

		std::shared_ptr<VulkModel> model{};
		glm::vec3 color{};
		TransformComponent transform{};

	private:
		VulkGameObj(id_t objId) : id{ objId } {};

		id_t id;
	};
}