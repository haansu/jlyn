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

		inline virtual void Init() {}
		inline virtual void Draw() {}

		inline std::string GetTitle() { return m_Title; }

	protected:
		std::string m_Title;
	};

}
