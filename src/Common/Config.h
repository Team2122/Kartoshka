/**
 * @file Config.h
 * @author Lee Bousfield
 * @date Jan 18, 2015
 */

#include <yaml-cpp/yaml.h>

#ifndef CONFIG_H_
#define CONFIG_H_

namespace tator {

/**
 * The Config class
 * Used for loading runtime values
 */
class Config {
protected:
	const constexpr static char* kConfigDir = "/etc/Kartoshka/";
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
