// Copyright (c) embedded ocean GmbH
#include "Input.hpp"

#include <xentara/data/Quality.hpp>
#include <xentara/model/Attribute.hpp>
#include <xentara/utils/string/cat.hpp>

#include <functional>
#include <stdexcept>

namespace xentara::plugins::templateMicroservice
{

auto Input::loadConfig(utils::json::decoder::Value &value, config::Resolver &resolver) -> void
{
	// Just submit a request
	resolver.submit<model::GenericElement>(value, std::ref(_element));
}

auto Input::prepare() -> void
{
	// Make sure the element was actually loaded
	auto element = _element.lock();
	if (!element)
	{
		return;
	}

	// Resolve the handles
	_value = readHandle(*element, model::Attribute::kValue.name());
	_quality = readHandle(*element, model::Attribute::kQuality.name());

	// Make sure the value is not write only
	if (_value == data::ReadHandle::Error::WriteOnly)
	{
		throw std::runtime_error(utils::string::cat("the value of element \"", element->primaryKey(), "\" is write only"));
	}
}

auto Input::readHandle(const model::GenericElement &element, std::string_view attributeName) -> data::ReadHandle
{
	// Get the handle
	auto handle = element.attributeReadHandle(attributeName);
	// Check that it exists
	if (handle == data::ReadHandle::Error::Unknown)
	{
		throw std::runtime_error(utils::string::cat("the element \"", element.primaryKey(), "\" does not have an attribute named \"", attributeName, '"'));
	}

	return handle;
}

auto Input::elementName() const -> std::string
{
	using namespace std::literals;

	// Get the element
	auto element = _element.lock();
	if (!element)
	{
		return "input"s;
	}

	return element->primaryKey();
}

auto Input::checkQuality() -> void
{
	// Read the quality
	auto quality = _quality.read<data::Quality>();
	if (!quality)
	{
		throw std::system_error(quality.error(), utils::string::cat("could not read quality of ", elementName()));
	}

	// Check it
	if (*quality > data::Quality::Acceptable)
	{
		throw std::runtime_error(utils::string::cat("quality of ", elementName(), " is ", toString(*quality)));
	}
}

auto Input::handleReadError(std::error_code error) -> void
{
	throw std::system_error(error, utils::string::cat("could not read ", elementName()));
}

} // namespace xentara::plugins::templateMicroservice