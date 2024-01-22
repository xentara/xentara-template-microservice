// Copyright (c) embedded ocean GmbH
#pragma once

#include <xentara/config/Context.hpp>
#include <xentara/data/ReadHandle.hpp>
#include <xentara/model/Element.hpp>
#include <xentara/utils/json/decoder/Value.hpp>

#include <memory>
#include <string>
#include <string_view>

namespace xentara::plugins::templateMicroservice
{

/// @brief A single input of the microservice
class Input final
{
public:
	/// @brief Loads the input from a configuration value
	auto load(utils::json::decoder::Value &value, config::Context &context) -> void;

	/// @brief Prepares the input
	auto prepare() -> void;

	/// @brief Reads the value as a certain type.
	/// @throws std::runtime_error An error occurred.
	template <typename Type>
	auto read() -> Type;

private:
	/// @brief Gets a read handle
	auto readHandle(const model::Element &element, std::string_view attributeName) -> data::ReadHandle;

	/// @brief Gets a name for the element for use in error messages
	auto elementName() const -> std::string;
	
	/// @brief Checks the quality.
	/// @throws std::runtime_error An error occurred.
	auto checkQuality() -> void;
	/// @brief Handles a read error. Always throws an exception.
	/// @throws std::runtime_error An exception of this type is always thrown.
	[[noreturn]] auto handleReadError(std::error_code error) -> void;

	/// @brief The xentara element representing the input we are reading
	std::weak_ptr<model::Element> _element;

	/// @brief The read handle for the quality of the input
	data::ReadHandle _quality;
	/// @brief The read handle for the value of the input
	data::ReadHandle _value;
};

template <typename Type>
auto Input::read() -> Type
{
	// Check the quality first
	checkQuality();

	// Try to read the value
	const auto value = _value.read<Type>();
	// Check it
	if (!value)
	{
		handleReadError(value.error());
	}

	return std::move(*value);
}

} // namespace xentara::plugins::templateMicroservice