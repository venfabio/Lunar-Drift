#include "Config.h"
#include "exceptions/Exceptions.h"
#include <fstream>
#include <cassert>
#include <sstream>
#include <algorithm>


/** @brief Replacement for std::to_string, which is unavailable at present. */
template <typename T>
std::string to_wstring(T val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}

/** @brief Replacement for std::trim, which is unavailable at present. */
std::string trim(std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last-first+1));
}


void Config::load(const std::string& file)
{
    assert(file.length() > 0);

	// Skip volatile internal config state container
	if (file.compare("_engine") == 0)
		return;

    const std::string filePath(file + ".ini");
    std::ifstream inFile(filePath.c_str());

    if(inFile.fail())
        throw FileIOException(filePath, "not found");

    std::string section("");
    while(!inFile.eof())
    {
        std::string entry;
        std::getline(inFile, entry);

        if(entry.length() <= 0)  // Exclude obvious blank lines
            continue;

        if(entry.at(0) == '#')  // Exclude comments
            continue;
        if(entry.at(0) == '[')  // Section declaration
        {
            std::size_t close = entry.find_first_of(']');
            if(close == std::string::npos)
                continue;
            section = entry.substr(1, close - 1);
            section.push_back(':');
            continue;
        }

        std::size_t split = entry.find_first_of('=');
        if(split == std::string::npos)
            continue;

        std::string key = entry.substr(0, split);
        std::string val = entry.substr(split+1, entry.length() - split);

        key = trim(key);
        val = trim(val);

        m_FileData[file][section + key] = val;
    }

    inFile.close();
}

void Config::save(const std::string& file)
{
    assert(file.length() > 0);

	// Skip volatile internal config state container
	if (file.compare("_engine") == 0)
		return;

    const std::string filePath(file + ".ini");
    std::ofstream outFile(filePath);

    if(outFile.fail())
        throw FileIOException(filePath, "not found");

    std::string section("");
    const std::map<std::string, std::string>& entries = m_FileData.at(file);
    for(const auto& entry : entries)
    {
        std::string key = entry.first;
        std::string val = entry.second;

        std::size_t splitLocation = key.find_first_of(':');
        if(splitLocation != std::string::npos)
        {
            std::string newSection = key.substr(0, splitLocation);
            key = key.substr(splitLocation+1, key.length() - splitLocation);
            if(section.compare(newSection) != 0)
            {
                section = newSection;
                outFile << std::endl << '[' << section << ']' << std::endl;
            }
        }

        outFile << key << "=" << val << std::endl;
    }

    outFile.close();
}

void Config::saveAll()
{
    for(const auto& file : m_FileData)
        save(file.first);
}

const std::string& Config::get(
    const std::string& file,
    const std::string& name) const
{
    assert(file.length() > 0);
    assert(name.length() > 0);

    if(m_FileData.count(file) == 0)
        throw ConfigurationException(file + ".ini", "not loaded");

    try
    {
        const std::map<std::string, std::string>& fileMap
            = m_FileData.at(file);

        return fileMap.at(name);
    } catch(std::out_of_range&) 
	{
        throw ConfigurationException(name, "not found in " + file);
    }
}

int Config::getInt(
    const std::string& file,
    const std::string& name) const
{
    assert(file.length() > 0);
    assert(name.length() > 0);

    int data;
    std::stringstream ss(get(file, name));
    ss >> data;
    if(!ss.good())
        throw ConfigurationException(file + "=>" + name,
                                     "is not an integer");

    return data;
}

bool Config::getBool(
    const std::string& file,
    const std::string& name,
    bool unknownIsFalse) const
{
    assert(file.length() > 0);
    assert(name.length() > 0);

    std::string word = get(file, name);
    std::transform(word.begin(), word.end(), word.begin(), tolower);

    if((word.compare("true") == 0)
       || (word.compare("1") == 0)
       || (word.compare("yes") == 0)
       || (word.compare("y") == 0))
    {
        return true;
    } else if((word.compare("false") == 0)
       || (word.compare("0") == 0)
       || (word.compare("no") == 0)
       || (word.compare("n") == 0))
    {
        return false;
    } else {
        if(!unknownIsFalse)
            throw ConfigurationException(file + "=>" + name,
                                         "is not a valid boolean value");
        return false;
    }
}

double Config::getDouble(
    const std::string& file,
    const std::string& name) const
{
    assert(file.length() > 0);
    assert(name.length() > 0);

    double data;
    std::stringstream ss(get(file, name));
    ss >> data;
    if(ss.fail() || ss.bad())
        throw ConfigurationException(file + "=>" + name,
                                     "is not a double-precision decimal");

    return data;
}

float Config::getFloat(
	const std::string& file,
	const std::string& name) const
{
	assert(file.length() > 0);
	assert(name.length() > 0);

	float data;
	std::stringstream ss(get(file, name));
	ss >> data;
	if (ss.fail() || ss.bad())
		throw ConfigurationException(file + "=>" + name,
									 "is not a floating-point decimal");

	return data;
}

void Config::set(
    const std::string& file,
    const std::string& name,
    const std::string value)
{
    assert(file.length() > 0);
    assert(name.length() > 0);

    m_FileData[file][name] = value;
}

void Config::set(
    const std::string& file,
    const std::string& name,
    int value)
{
    assert(file.length() > 0);
    assert(name.length() > 0);

    set(file, name, to_wstring(value));
}

void Config::set(
    const std::string& file,
    const std::string& name,
    bool value)
{
    assert(file.length() > 0);
    assert(name.length() > 0);

    std::string result (value ? "true" : "false");
    set(file, name, result);
}

void Config::set(
    const std::string& file,
    const std::string& name,
    double value)
{
    assert(file.length() > 0);
    assert(name.length() > 0);

    set(file, name, to_wstring(value));
}
