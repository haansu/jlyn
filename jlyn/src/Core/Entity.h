#pragma once

#include <JLynCore.h>

namespace jlyn {

	class Entity {
	public:
		Entity()
			: m_Title("Entity") {}
		Entity(std::string _title)
			: m_Title(_title) {}
		virtual ~Entity() = default;

		virtual void Init();

	protected:
		std::string m_Title;
	};

}
