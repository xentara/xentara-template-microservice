// Copyright (c) embedded ocean GmbH
#pragma once

#include <xentara/process/Event.hpp>

/// @brief Contains the Xentara events of the microservice
namespace xentara::plugins::templateMicroservice::events
{

/// @brief A Xentara event that is raised when the microservice instance was executed correctly
extern const process::Event::Role kExecuted;
/// @brief A Xentara event that is raised an error occurs executing the microservice instance
extern const process::Event::Role kExecutionError;

} // namespace xentara::plugins::templateMicroservice::events