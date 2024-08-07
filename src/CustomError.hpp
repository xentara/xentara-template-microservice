// Copyright (c) embedded ocean GmbH
#pragma once

#include <system_error>
#include <type_traits>

namespace xentara::plugins::templateMicroservice
{

/// @brief Custom error codes used by the driver
/// @todo Add error codes for other custom errors
enum class CustomError
{
	/// @brief No error has occurred. This has the value 0 to conform to the conventions of std::error_code.
	NoError = 0,

	/// @brief The microservice instance has not been executed yet
	Pending,
	/// @brief The microservice instance is in the process of being suspended
	Suspending,
	/// @brief The microservice instance is suspended
	Suspended,

	/// @brief An unknown error occurred
	UnknownError = 999
};

/// @brief The error category for custom errors
auto customErrorCategory() noexcept -> const std::error_category &;

/// @brief Support automatic conversion from @ref CustomError to std::error_code
inline auto make_error_code(CustomError error) noexcept -> std::error_code
{
	return { int(error), customErrorCategory() };
}

} // namespace xentara::plugins::templateMicroservice

/// @brief This specialization enables automatic conversion from xentara::plugins::templateMicroservice::CustomError to std::error_code.
template<>
struct std::is_error_code_enum<xentara::plugins::templateMicroservice::CustomError> : public std::true_type
{
};
