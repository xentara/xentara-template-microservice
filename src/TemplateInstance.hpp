// Copyright (c) embedded ocean GmbH
#pragma once

#include "Attributes.hpp"
#include "CustomError.hpp"
#include "Input.hpp"
#include "Output.hpp"

#include <xentara/memory/Array.hpp>
#include <xentara/process/Event.hpp>
#include <xentara/process/Task.hpp>
#include <xentara/skill/Element.hpp>
#include <xentara/skill/EnableSharedFromThis.hpp>
#include <xentara/utils/core/Uuid.hpp>

#include <functional>
#include <string>
#include <string_view>
#include <optional>
#include <new>

namespace xentara::plugins::templateMicroservice
{

using namespace std::literals;

class TemplateClient;

/// @brief A class representing an instance of the microservice.
/// @todo rename this class to something more descriptive
class TemplateInstance final : public skill::Element, public skill::EnableSharedFromThis<TemplateInstance>
{
private:
	/// @brief A structure used to store the class specific attributes within an element's configuration
	struct Config final
	{
		/// @todo Add custom config attributes
	};
	
public:
	/// @brief The class object containing meta-information about this element type
	class Class final : public skill::Element::Class
	{
	public:
		/// @brief Gets the global object
		static auto instance() -> Class&
		{
			return _instance;
		}

	    /// @brief Returns the array handle for the class specific attributes within an element's configuration
	    auto configHandle() const -> const auto &
        {
            return _configHandle;
        }

		/// @name Virtual Overrides for skill::Element::Class
		/// @{

		auto name() const -> std::string_view final
		{
			/// @todo change class name
			return "TemplateInstance"sv;
		}
	
		auto uuid() const -> utils::core::Uuid final
		{
			/// @todo assign a unique UUID
			return "deadbeef-dead-beef-dead-beefdeadbeef"_uuid;
		}

		/// @}

	private:
	    /// @brief The array handle for the class specific attributes within an element's configuration
		memory::Array::ObjectHandle<Config> _configHandle { config().appendObject<Config>() };

		/// @brief The global object that represents the class
		static Class _instance;
	};

	/// @name Virtual Overrides for skill::Element
	/// @{

	auto forEachAttribute(const model::ForEachAttributeFunction &function) const -> bool final;
	
	auto forEachEvent(const model::ForEachEventFunction &function) -> bool final;

	auto forEachTask(const model::ForEachTaskFunction &function) -> bool final;

	auto makeReadHandle(const model::Attribute &attribute) const noexcept -> std::optional<data::ReadHandle> final;

	auto realize() -> void final;

	auto prepare() -> void final;
	
	/// @}

protected:
	/// @name Virtual Overrides for skill::Element
	/// @{

	auto loadConfig(const ConfigIntializer &initializer,
		utils::json::decoder::Object &jsonObject,
		config::Resolver &resolver,
		const config::FallbackHandler &fallbackHandler) -> void final;
	
	/// @}

private:
	/// @brief This structure represents the current state of the instance
	struct State final
	{
		/// @brief Whether the instance is being executed correctly
		bool _executionState { false };
		/// @brief The last time the instance was executed (successfully or not)
		std::chrono::system_clock::time_point _executionTime { std::chrono::system_clock::time_point::min() };
		/// @brief The error message, or a default constructed object for none.
		std::error_code _error { CustomError::Pending };
	};

	/// @brief This class provides callbacks for the Xentara scheduler for the "execute" task
	class ExecuteTask final : public process::Task
	{
	public:
		/// @brief This constuctor attached the task to its target
		ExecuteTask(std::reference_wrapper<TemplateInstance> target) : _target(target)
		{
		}

		/// @name Virtual Overrides for process::Task
		/// @{

		auto stages() const -> Stages final
		{
			return Stage::PreOperational | Stage::Operational | Stage::PostOperational;
		}

		auto preparePreOperational(const process::ExecutionContext &context) -> Status final;

		auto operational(const process::ExecutionContext &context) -> void final;

		auto preparePostOperational(const process::ExecutionContext &context) -> Status final;

		/// @}

	private:
		/// @brief A reference to the microservice instance
		std::reference_wrapper<TemplateInstance> _target;
	};

	/// @brief This function is called by the "execute" task on startup.
	auto prePerformExecuteTask(const process::ExecutionContext &context) -> void;
	/// @brief This function is called by the "execute" task.
	auto performExecuteTask(const process::ExecutionContext &context) -> void;
	/// @brief This function is called by the "execute" task on shutdown.
	auto postPerformExecuteTask(const process::ExecutionContext &context) -> void;

	/// @brief Executes the microservice.
	/// @throws std::runtime_error An error occurred
	auto execute(std::chrono::system_clock::time_point timeStamp) -> void;
	/// @brief Safes the state.
	/// @return Returns an error code on error, or a default constructed error code object on success.
	auto safe(std::chrono::system_clock::time_point timeStamp) -> std::error_code;

	/// @brief Updates the state
	auto updateState(std::chrono::system_clock::time_point timeStamp, std::error_code error = std::error_code()) -> void;

	/// @brief A Xentara event that is fired when the microservice has been executed successfully
	process::Event _executedEvent;
	/// @brief A Xentara event that is fired when an error occurred
	process::Event _errorEvent;

	/// @brief The "execute" task
	ExecuteTask _executeTask { *this };

	/// @brief The data block that contains the state
	memory::ObjectBlock<State> _stateDataBlock;

	/// @brief An input written by the microiservice
	/// @todo give this a more descriptive name, and add a member variable for each supported input
	Input _templateInput;

	/// @brief An output written by the microiservice
	/// @todo give this a more descriptive name, and add a member variable for each supported output
	Output _templateOutput;
};

} // namespace xentara::plugins::templateMicroservice