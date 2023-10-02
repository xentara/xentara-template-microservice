// Copyright (c) embedded ocean GmbH
#pragma once

#include "TemplateInstance.hpp"

#include <xentara/skill/Skill.hpp>
#include <xentara/utils/core/Uuid.hpp>

#include <string_view>

namespace xentara::plugins::templateMicroservice
{

using namespace std::literals;

/// @brief The skill
class Skill final : public skill::Skill
{
public:
	/// @class xentara::plugins::templateMicroservice::Skill
	/// @todo add constructor to perform global initialization, if necessary

	/// @class xentara::plugins::templateMicroservice::Skill
	/// @todo add destructor to perform global cleanup, if necessary

	/// @name Virtual Overrides for skill::Skill
	/// @{

	auto createElement(const skill::Element::Class &elementClass, skill::ElementFactory &factory)
		-> std::shared_ptr<skill::Element> final;

	/// @}

private:
	/// @brief The skill class
	/// @todo change class name
	/// @todo assign a unique UUID
	using Class = ConcreteClass<Skill,
		"TemplateMicroservice",
		"deadbeef-dead-beef-dead-beefdeadbeef"_uuid,
		TemplateInstance::Class>;

	/// @brief The skill class object
	static Class _class;
};

} // namespace xentara::plugins::templateMicroservice