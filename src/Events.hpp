// Copyright (c) embedded ocean GmbH
#pragma once

#include <xentara/process/Event.hpp>

/// @brief Contains the Xentara events of the microservice
namespace xentara::plugins::templateMicroservice::events
{

/// @brief A Xentara event that is fired when the microservice instance was executed correctly
extern const process::Event::Role kExecuted;
/// @brief A Xentara event that is fired an error occurs executing the microservice instance
extern const process::Event::Role kError;

} // namespace xentara::plugins::templateMicroservice::events