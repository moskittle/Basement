#pragma once

#include "Basement/Core/Time.h"

#include <entt/entt.hpp>

namespace Basement
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void Init();
		void Update(const Timer& dt);
		void Cleanup();

	private:
		entt::registry m_Registry;

	};

}