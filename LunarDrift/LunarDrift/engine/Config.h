#pragma once

#include <string>
#include <map>

/**
 * @brief A singleton that aggregates and encaspsulates access to config files
 *
 * Most projects utilize .INI files to configure the program. They provide
 * convenient storage places for configuration variables, and are both easily
 * readable and modifiable by users from outside of the program.
 * This singleton class allows these INI files to be conveniently handled and
 * interfaced for easier use by programmers.
 *
 * @note "_engine" file is a virtual file for representing internal engine configuration states
 *
 * @author Hayley Hatton
 * @date   20/02/2016
 * @ingroup TopLevelContainers
 */
class Config
{
public:
    /**
     * @brief Access the singleton instance
     * @return The actual Config instance
     */
    static Config& get()
    {
        static Config instance;
        return instance;
    }

    /**
     * @brief Load an INI file into the configuration settings database
     * @param file Name of the INI file, without the .ini extension
     * @throw FileIOException
     */
    void load(const std::string& file);

    /**
     * @brief Save to an INI file from the configuration settings database
     * @param file Name of the INI file, without the .ini extension
     * @throw FileIOException
     */
    void save(const std::string& file);

    /**
     * @brief Save all loaded configuration files with current settings
     */
    void saveAll();

    /**
     * @brief Access a setting from a loaded configuration file
     * @param file Name of the INI file, without the .ini extension
     * @param name Configuration setting to query
     * @return String value associated with the queried setting
     * @throw ConfigurationException
     */
    const std::string& get(
        const std::string& file,
        const std::string& name) const;

    /**
     * @brief Access a setting from a loaded configuration file
     * @param file Name of the INI file, without the .ini extension
     * @param name Configuration setting to query
     * @return Signed integer associated with the queried setting
     * @throw ConfigurationException
     */
    int getInt(
        const std::string& file,
        const std::string& name) const;

    /**
     * @brief Access a setting from a loaded configuration file
     * @param file Name of the INI file, without the .ini extension
     * @param name Configuration setting to query
     * @param unknownIsFalse If false, throw exception if setting is false
     * @return True/false value associated with the queried setting
     * @throw ConfigurationException
     */
    bool getBool(
        const std::string& file,
        const std::string& name,
        bool unknownIsFalse=true) const;

    /**
     * @brief Access a setting from a loaded configuration file
     * @param file Name of the INI file, without the .ini extension
     * @param name Configuration setting to query
     * @return Double-precision decimal associated with the queried setting
     * @throw ConfigurationException
     */
    double getDouble(
        const std::string& file,
		const std::string& name) const;

	/**
	* @brief Access a setting from a loaded configuration file
	* @param file Name of the INI file, without the .ini extension
	* @param name Configuration setting to query
	* @return Floating-point decimal associated with the queried setting
	* @throw ConfigurationException
	*/
	float getFloat(
		const std::string& file,
		const std::string& name) const;

    /**
     * @brief Set a setting in the configuration database
     * @param file Name of the INI file, without the .ini extension
     * @param name Configuration setting to set value of
     * @param value String to set the setting to
     */
    void set(
        const std::string& file,
        const std::string& name,
        const std::string value);

    /**
     * @brief Set a setting in the configuration database
     * @param file Name of the INI file, without the .ini extension
     * @param name Configuration setting to set value of
     * @param value Signed integer to set the setting to
     */
    void set(
        const std::string& file,
        const std::string& name,
        int value);

    /**
     * @brief Set a setting in the configuration database
     * @param file Name of the INI file, without the .ini extension
     * @param name Configuration setting to set value of
     * @param value True/false value to set the setting to
     */
    void set(
        const std::string& file,
        const std::string& name,
        bool value);

    /**
     * @brief Set a setting in the configuration database
     * @param file Name of the INI file, without the .ini extension
     * @param name Configuration setting to set value of
     * @param value Double-precision decimal to set the setting to
     */
    void set(
        const std::string& file,
        const std::string& name,
        double value);

private:
    std::map<std::string,
                std::map<std::string,
                        std::string> > m_FileData; //!< Settings database

    // Singleton constructor, destructor and operator blockers
    Config() {};
    Config(Config const&);
    void operator=(Config const&);
};
