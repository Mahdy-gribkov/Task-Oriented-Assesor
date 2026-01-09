#ifndef VANGUARD_SD_MANAGER_H
#define VANGUARD_SD_MANAGER_H

/**
 * @file SDManager.h
 * @brief Handles SD card initialization and file operations
 */

#include <Arduino.h>
#include <SD.h>
#include <FS.h>

namespace Vanguard {

class SDManager {
public:
    static SDManager& getInstance();

    bool init();
    bool isAvailable() const;

    // Folder management
    bool ensureDirectory(const char* path);
    void createFolderStructure();

    // File operations
    bool appendToFile(const char* path, const char* data);
    String readFile(const char* path);
    bool fileExists(const char* path);
    
    // Logging helpers
    bool logCredential(const char* ssid, const char* user, const char* pass, const char* mac);

private:
    SDManager();
    bool m_initialized;
};

} // namespace Vanguard

#endif // VANGUARD_SD_MANAGER_H
