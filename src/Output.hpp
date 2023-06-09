// Copyright (c) embedded ocean GmbH
#pragma once

#include <xentara/config/Resolver.hpp>
#include <xentara/data/WriteHandle.hpp>
#include <xentara/model/Element.hpp>
#include <xentara/utils/json/decoder/Value.hpp>

#include <memory>
#include <string>
#include <new>

namespace xentara::plugins::templateMicroservice
{

/// @brief A single output of the microservice
class Output
{
public:
	/// @brief Loads the output from a configuration value
	auto loadConfig(utils::json::decoder::Value &value, config::Resolver &resolver) -> void;

	/// @brief Prepares the output
	auto prepare() -> void;

	/// @brief Writes the value as a certain type.
	/// @throws std::runtime_error An error occurred.
	template <typename Type>
	auto write(const Type &value) -> void;

	/// @brief Writes the value as a certain type without throwing any errors.
	/// @return Returns an error code on error, or a default constructed error code object on success.
	template <typename Type>
	auto write(const Type &value, std::nothrow_t) -> std::error_code;

private:
	/// @brief Gets a write handle
	auto writeHandle(model::Element &element, std::string_view attributeName) -> data::WriteHandle;

	/// @brief Gets a name for the element for use in error messages
	auto elementName() const -> std::string;
	
	/// @brief Handles a write error
	/// @throws std::runtime_error An exception of this type is always thrown.
	[[noreturn]] auto handleWriteError(std::error_code error) -> void;

	/// @brief The xentara element representing the output we are supposed to write
	std::weak_ptr<model::Element> _element;

	/// @brief The write handle for the value of the output
	data::WriteHandle _value;
};

template <typename Type>
auto Output::write(const Type &value) -> void
{
	// Try to read the value
	const auto error = _value.write<Type>(value);
	// Handles errors
	if (error)
	{
		handleWriteError(error);
	}
}

template <typename Type>
auto Output::write(const Type &value, std::nothrow_t) -> std::error_code
{
	return _value.write<Type>(value);
}

} // namespace xentara::plugins::templateMicroservice