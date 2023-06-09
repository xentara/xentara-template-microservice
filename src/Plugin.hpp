// Copyright (c) embedded ocean GmbH
#pragma once

#include "ServiceProvider.hpp"

#include <xentara/plugin/Plugin.hpp>

/// @namespace xentara
/// @brief base namespace for Xentara

/// @namespace xentara::plugins
/// @brief base namespace for Xentara plugins

/// @brief base namespace for the microservice skill
/// @todo rename namespace
namespace xentara::plugins::templateMicroservice
{

/// @brief The class that registers the library as a Xentara plugin.
class Plugin final : plugin::Plugin
{
public:
	/// @name Virtual Overrides for plugin::Plugin
	/// @{

	auto registerObjects(Registry & registry) -> void final
	{
		// Register the service provider.
		registry << _serviceProvider;
	}
	
	/// @}

private:
	/// @brief The service provider
	ServiceProvider _serviceProvider;

	/// @brief The global plugin object
	static Plugin _instance;
};

} // namespace xentara::plugins::templateMicroservice
