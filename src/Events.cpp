// Copyright (c) embedded ocean GmbH
#include "Events.hpp"

#include <xentara/utils/core/Uuid.hpp>

#include <string_view>

namespace xentara::plugins::templateMicroservice::events
{

using namespace std::literals;
using namespace xentara::literals;

/// @todo assign a unique UUID
const process::Event::Role kExecuted { "deadbeef-dead-beef-dead-beefdeadbeef"_uuid, "executed"sv };

/// @todo assign a unique UUID
const process::Event::Role kError { "deadbeef-dead-beef-dead-beefdeadbeef"_uuid, "error"sv };

} // namespace xentara::plugins::templateMicroservice::events