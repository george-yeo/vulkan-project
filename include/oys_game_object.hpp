#pragma once

#include <oys_model.hpp>

// std
#include <memory>

namespace oys {

	struct Transform2dComponent {
		glm::vec2 translation{};
		glm::vec2 scale{ 1.f, 1.f };
		float rotation;

		glm::mat2 mat2() {
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotMatrix({ c, s }, { -s,c });

			glm::mat2 scaleMat({ scale.x, .0f }, { .0f, scale.y });
			return rotMatrix * scaleMat;
		}
	};

	class OysGameObject {
	public:
		using id_t = unsigned int;

		static OysGameObject createGameObject() {
			static id_t currentId = 0;

			return OysGameObject(currentId++);
		}

		OysGameObject(const OysGameObject&) = delete;
		OysGameObject& operator=(const OysGameObject&) = delete;
		OysGameObject(OysGameObject&&) = default;
		OysGameObject& operator=(OysGameObject&&) = default;

		id_t getId() { return id; }

		std::shared_ptr<OysModel> model{};
		glm::vec3 color{};
		Transform2dComponent transform2d{};

	private:
		OysGameObject(id_t objId) : id{ objId } {};

		id_t id;
	};
}