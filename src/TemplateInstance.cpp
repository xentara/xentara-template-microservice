// Copyright (c) embedded ocean GmbH
#include "TemplateInstance.hpp"

#include "Attributes.hpp"
#include "Events.hpp"
#include "Tasks.hpp"

#include <xentara/config/FallbackHandler.hpp>
#include <xentara/data/DataType.hpp>
#include <xentara/data/ReadHandle.hpp>
#include <xentara/data/WriteHandle.hpp>
#include <xentara/memory/memoryResources.hpp>
#include <xentara/memory/WriteSentinel.hpp>
#include <xentara/model/Attribute.hpp>
#include <xentara/model/ForEachAttributeFunction.hpp>
#include <xentara/model/ForEachEventFunction.hpp>
#include <xentara/model/ForEachTaskFunction.hpp>
#include <xentara/process/EventList.hpp>
#include <xentara/process/ExecutionContext.hpp>
#include <xentara/utils/json/decoder/Object.hpp>
#include <xentara/utils/json/decoder/Errors.hpp>
#include <xentara/utils/eh/currentErrorCode.hpp>

namespace xentara::plugins::templateMicroservice
{
	
using namespace std::literals;

TemplateInstance::Class TemplateInstance::Class::_instance;

auto TemplateInstance::loadConfig(const ConfigIntializer &initializer,
		utils::json::decoder::Object &jsonObject,
		config::Resolver &resolver,
		const config::FallbackHandler &fallbackHandler) -> void
{
	// Keep track of which inputs/outputs have been loaded
	// @todo add more loaded flags for all required inputs and outputs
	bool templateInputLoaded = false;
	bool templateOutputLoaded = false;

	// Go through all the members of the JSON object that represents this object
	for (auto && [name, value] : jsonObject)
    {
		/// @todo use a more descriptive keyword, and add a block for each supported input
		if (name == "templateInput")
		{
			_templateInput.loadConfig(value, resolver);
			templateInputLoaded = true;
		}
		/// @todo use a more descriptive keyword, and add a block for each supported output
		if (name == "templateOutput")
		{
			_templateOutput.loadConfig(value, resolver);
			templateOutputLoaded = true;
		}
		/// @todo load custom configuration parameters
		else if (name == "TODO"sv)
		{
			/// @todo parse the value correctly
			auto todo = value.asNumber<std::uint64_t>();

			/// @todo check that the value is valid
			if (!"TODO")
			{
				/// @todo use an error message that tells the user exactly what is wrong
				utils::json::decoder::throwWithLocation(value, std::runtime_error("TODO is wrong with TODO parameter of template transaction"));
			}

			/// @todo set the appropriate member variables, and update configAttributes accordingly (if necessary) 
		}
		else
		{
			// Pass any unknown parameters on to the fallback handler, which will load the built-in parameters ("id", "uuid", and "children"),
			// and throw an exception if the key is unknown
            fallbackHandler(name, value);
		}
    }

	// Check that all inputs and outputs have been loaded
	/// @todo add a block for each required input
	if (!templateInputLoaded)
	{
		/// @todo replace "template input" and "template microservice instance" with more descriptive names
		utils::json::decoder::throwWithLocation(jsonObject, std::runtime_error("no template input specified for template microservice instance"));
	}
	/// @todo add a block for each required output
	if (!templateOutputLoaded)
	{
		/// @todo replace "template input" and "template microservice instance" with more descriptive names
		utils::json::decoder::throwWithLocation(jsonObject, std::runtime_error("no right input specified for template microservice instance"));
	}
	/// @todo perform additional consistency and completeness checks
	if (!"TODO")
	{
		/// @todo use an error message that tells the user exactly what is wrong
		utils::json::decoder::throwWithLocation(jsonObject, std::runtime_error("TODO is wrong with template microservice instance"));
	}
}

auto TemplateInstance::performExecuteTask(const process::ExecutionContext &context) -> void
{
	// Get the time stamp
	const auto timeStamp = context.scheduledTime();

	try
	{
		// execute the task
		execute(timeStamp);
		// The execution was successful
		updateState(timeStamp);
	}
	catch (const std::exception &exception)
	{
		// Update the state
		updateState(timeStamp, utils::eh::currentErrorCode());
	}
}

auto TemplateInstance::prePerformExecuteTask(const process::ExecutionContext &context) -> void
{
	// We are now pending
	updateState(context.scheduledTime(), CustomError::Pending);
}

auto TemplateInstance::postPerformExecuteTask(const process::ExecutionContext &context) -> void
{
	// Get the time stamp
	const auto timeStamp = context.scheduledTime();

	// safe the state
	const auto error = safe(timeStamp);
	// Check for errors
	if (error)
	{
		updateState(timeStamp, error);
		return;
	}

	// We are now suspended
	updateState(timeStamp, CustomError::Suspended);
}

auto TemplateInstance::execute(std::chrono::system_clock::time_point timeStamp) -> void
{
	try
	{
		// Read the inputs
		/// @todo use the correct type to read the template input
		const auto templateInputValue = _templateInput.read<double>();

		/// @todo read any additional required inputs

		/// @todo write any outputs that need to be written
		_templateOutput.write(double());
	}
	catch (...)
	{
		// Try to safe the state, but ignore any further errors
		safe(timeStamp);

		throw;
	}
}

auto TemplateInstance::safe(std::chrono::system_clock::time_point timeStamp) -> std::error_code
{
	// An error that occurred safing
	std::error_code error;
	// This is a helper function to track the error. It only remembers the first error that occurred.
	const auto handleError = [&error](std::error_code newError)
	{
		if (!error)
		{
			error = newError;
		}
	};

	/// @todo write all outputs that need to be written to safe the state
	handleError(_templateOutput.write(double(), std::nothrow));

	return error;
}

auto TemplateInstance::updateState(std::chrono::system_clock::time_point timeStamp, std::error_code error) -> void
{
	// Make a write sentinel
	memory::WriteSentinel sentinel { _stateDataBlock };
	auto &state = *sentinel;

	// Update the state
	state._executionState = !error;
	state._executionTime = timeStamp;
	state._error = error;

	// Determine the correct event
	const auto &event = error ? _executionErrorEvent : _executedEvent;
	// Commit the data and raise the event
	sentinel.commit(timeStamp, event);
}

auto TemplateInstance::forEachAttribute(const model::ForEachAttributeFunction &function) const -> bool
{
	/// @todo handle any additional attributes this class supports, including attributes inherited from the client
	return
		function(attributes::kExecutionState) ||
		function(attributes::kExecutionTime) ||
		function(attributes::kError);
}

auto TemplateInstance::forEachEvent(const model::ForEachEventFunction &function) -> bool
{
	// Handle all the events we support
	return
		function(events::kExecuted, sharedFromThis(&_executedEvent)) ||
		function(events::kExecutionError, sharedFromThis(&_executionErrorEvent));

	/// @todo handle any additional events this class supports
}

auto TemplateInstance::forEachTask(const model::ForEachTaskFunction &function) -> bool
{
	// Handle all the tasks we support
	return
		function(tasks::kExecute, sharedFromThis(&_executeTask));

	/// @todo handle any additional tasks this class supports
}

auto TemplateInstance::makeReadHandle(const model::Attribute &attribute) const noexcept -> std::optional<data::ReadHandle>
{
	// Try our attributes
	if (attribute == attributes::kExecutionState)
	{
		return _stateDataBlock.member(&State::_executionState);
	}
	else if (attribute == attributes::kExecutionTime)
	{
		return _stateDataBlock.member(&State::_executionTime);
	}
	else if (attribute == attributes::kError)
	{
		return _stateDataBlock.member(&State::_error);
	}

	/// @todo add support for any additional attributes, including attributes inherited from the client

	return std::nullopt;
}

auto TemplateInstance::realize() -> void
{
	// Create the data block
	_stateDataBlock.create(memory::memoryResources::data());
}

auto TemplateInstance::prepare() -> void
{
	// Prepare all the inputs and outputs
	_templateInput.prepare();
	_templateOutput.prepare();

	/// @todo call prepare() for any additional supported inputs and outputs
}

auto TemplateInstance::ExecuteTask::preparePreOperational(const process::ExecutionContext &context) -> Status
{
	_target.get().prePerformExecuteTask(context);
	return Status::Completed;
}

auto TemplateInstance::ExecuteTask::operational(const process::ExecutionContext &context) -> void
{
	_target.get().performExecuteTask(context);
}

auto TemplateInstance::ExecuteTask::preparePostOperational(const process::ExecutionContext &context) -> Status
{
	_target.get().postPerformExecuteTask(context);
	return Status::Completed;
}

} // namespace xentara::plugins::templateMicroservice