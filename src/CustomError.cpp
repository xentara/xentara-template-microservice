// Copyright (c) embedded ocean GmbH
#include "CustomError.hpp"

#include <system_error>
#include <string>

namespace xentara::plugins::templateMicroservice
{

namespace
{

	/// @brief This is a custom error category to encapsulate @ref CustomError errors in std::error_code objects
	class CustomErrorCategory final : public std::error_category
	{
	public:
		
		/// @name Virtual Overrides for std::error_category
		/// @{
		/// 
		auto name() const noexcept -> const char * final;

		auto message(int errorCode) const -> std::string final;
		
		/// @}
	};

	auto CustomErrorCategory::name() const noexcept -> const char *
	{
		/// @todo Rename error cotegory
		return "Xentara.templateMicroservice";
	}

	auto CustomErrorCategory::message(int errorCode) const -> std::string
	{
		using namespace std::literals;

		// Check the error code
		switch (CustomError(errorCode))
		{
		case CustomError::NoError:
			return "success"s;

		case CustomError::Pending:
			return "the microservice instance has not been executed yet"s;

		case CustomError::Suspending:
			return "the microservice instance is in the process of being suspended"s;

		case CustomError::Suspended:
			return "the microservice instance is suspended"s;

		/// @todo Add messages for other error codes

		case CustomError::UnknownError:
		default:
			return "an unknown error occurred"s;
		}
	}

} // namespace

auto customErrorCategory() noexcept -> const std::error_category &
{
	// Return a static object
	static const CustomErrorCategory kErrorCategory;
	return kErrorCategory;
}

} // namespace xentara::plugins::templateMicroservice