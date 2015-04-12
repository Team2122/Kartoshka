/**
 * @file Config.h
 * @author Lee Bousfield
 * @date Jan 18, 2015
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <yaml-cpp/yaml.h>

namespace tator {

/**
 * The Config class
 * Used for loading runtime values
 */
class Config {
protected:
	const constexpr static char* kConfigDir = "/etc/Kartoshka/";

	static void ApplyOverrides(YAML::Node to, YAML::Node from);
public:
	/// Subsystems config
	static YAML::Node subsystems;
	/// Commands config
	static YAML::Node commands;
	/// Triggers config
	static YAML::Node triggers;

	/**
	 * Load config
	 */
	static void Load();
	/**
	 * Config will no longer exist
	 */
	static void Delete();
};

}

#endif /* CONFIG_H_ */
