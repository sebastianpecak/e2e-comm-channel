#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <map>
#include <string>

class Configuration
{
private:
    /**
     * Configuration is stored in key-value fashion.
     */
    std::map<std::string, std::string> _keyValueStore;

public:
    /**
     * Initialize object with command line arguments.
     */
    void ParseCliArguments(int argc, char **argv);
    /**
     * Get configuration key's value.
     * If key does not exist it returns empty string.
     * Use Exists method to check if key is set.
     */
    std::string Get(const std::string &key) const;
    /**
     * Set key' value.
     */
    void Set(const std::string &key, const std::string &value);
    /**
     * Return true if given key is set.
     */
    bool Exsist(const std::string &key) const;
};

#endif // CONFIGURATION_H
