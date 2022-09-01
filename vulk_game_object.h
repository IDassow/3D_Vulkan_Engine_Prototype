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

		glm::mat4 mat4();
		glm::mat3 normalMatrix();
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