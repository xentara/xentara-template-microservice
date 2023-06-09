// Copyright (c) embedded ocean GmbH
#include "Skill.hpp"

#include <xentara/skill/Element.hpp>
#include <xentara/skill/ElementFactory.hpp>

namespace xentara::plugins::templateMicroservice
{

auto Skill::createElement(const skill::Element::Class &elementClass, skill::ElementFactory &factory)
	-> std::shared_ptr<skill::Element>
{
	if (&elementClass == &TemplateInstance::Class::instance())
	{
		return factory.makeShared<TemplateInstance>();
	}

	/// @todo handle any additional top-level microservice classes

	return nullptr;
}

} // namespace xentara::plugins::templateMicroservice