#ifndef HESTIA_UTIL_LOGGER_HPP_
#define HESTIA_UTIL_LOGGER_HPP_

#include <iostream>
#include <string>

namespace HGE {

    // todo: Update to singleton

    /**
     * Log a debug message to console
     * TODO: Add array of items that will follow?
     * TODO: Add to separate thread
     */
    static void LogDebug(std::string tag, std::string msg) {
        std::cout << "DEBUG: " << tag << " - " << msg << "\n";
    }

    static void LogError(std::string tag, std::string msg) {
        std::cout << "ERROR: " << tag << " - " << msg << "\n";
    }
}
#endif