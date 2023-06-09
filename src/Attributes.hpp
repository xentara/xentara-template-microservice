// Copyright (c) embedded ocean GmbH
#pragma once

#include <xentara/model/Attribute.hpp>

/// @brief Contains the Xentara attributes of the uplink skill
namespace xentara::plugins::templateMicroservice::attributes
{

/// @brief A Xentara attribute containing the execution state of a microservice instance
extern const model::Attribute kExecutionState;
/// @brief A Xentara attribute containing the execution time of a microservice instance
extern const model::Attribute kExecutionTime;
/// @brief A Xentara attribute containing an error message for a microservice instance
extern const model::Attribute kError;

} // namespace xentara::plugins::templateMicroservice::attributes
