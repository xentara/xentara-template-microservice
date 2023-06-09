// Copyright (c) embedded ocean GmbH
#include "Attributes.hpp"

#include <xentara/utils/core/Uuid.hpp>
#include <xentara/data/DataType.hpp>

#include <string_view>

namespace xentara::plugins::templateMicroservice::attributes
{

using namespace std::literals;

/// @todo assign a unique UUID
const model::Attribute kExecutionState { "deadbeef-dead-beef-dead-beefdeadbeef"_uuid, "executionState"sv, model::Attribute::Access::ReadOnly, data::DataType::kBoolean };

/// @todo assign a unique UUID
const model::Attribute kExecutionTime { "deadbeef-dead-beef-dead-beefdeadbeef"_uuid, "executionTime"sv, model::Attribute::Access::ReadOnly, data::DataType::kTimeStamp };

const model::Attribute kError { model::Attribute::kError, model::Attribute::Access::ReadOnly, data::DataType::kErrorCode };

} // namespace xentara::plugins::templateMicroservice::attributes
