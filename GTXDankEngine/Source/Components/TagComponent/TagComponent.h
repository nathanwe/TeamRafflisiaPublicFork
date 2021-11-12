#ifndef TAG_COMPONENT_H
#define TAG_COMPONENT_H

#include "pch.h"
#include "../Core/ComponentPool.h"

class TagComponent : public AbstractComponent
{
public:
	TagComponent(Entity e) : uniqueName(""), category(""), AbstractComponent(e) {}
	std::string uniqueName;
	std::string category;
};

inline void to_json(ordered_json& j, const TagComponent& tagComponent) {
	to_json(j["uniqueName"], tagComponent.uniqueName);
	to_json(j["category"], tagComponent.category);
}

inline void from_json(const ordered_json& j, TagComponent& tagComponent) {
	from_json(j["uniqueName"], tagComponent.uniqueName);
	from_json(j["category"], tagComponent.category);
}

extern AbstractComponentPool<TagComponent> TagComponentPool;

#endif
