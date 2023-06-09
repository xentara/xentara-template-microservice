// Copyright (c) embedded ocean GmbH
#include "Tasks.hpp"

#include <xentara/utils/core/Uuid.hpp>

#include <string_view>

namespace xentara::plugins::templateMicroservice::tasks
{

using namespace std::literals;
using namespace xentara::literals;

/// @todo assign a unique UUID
const process::Task::Role kExecute { "deadbeef-dead-beef-dead-beefdeadbeef"_uuid, "execute"sv };

} // namespace xentara::plugins::templateMicroservice::tasks