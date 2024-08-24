#pragma once
#include "types.h"
#include "VkVertexParser.h"
#include <memory>
namespace engine
{
	struct TransformComponent
	{
		glm::vec2 translation = {};
		glm::mat2 mat2() { return { 1.0f }; }
	};


	class GameObject
	{
	public:
		static GameObject CreateGameObject()
		{
			static id_t currentId = 0;
			return {currentId++};
		}
		GameObject& operator=(const GameObject&) = delete;
		GameObject(const GameObject&) = delete;
		GameObject &operator=(GameObject&&) = default;
		GameObject(GameObject&&) = default;
		id_t GetId() const { return id; }

		std::shared_ptr<VkVertexParser> parser = {};
		glm::vec3 color = {};
		TransformComponent Transform2D = {};
	private:
		GameObject(id_t objId) : id{ objId } {}
		id_t id;
	};

}