#ifndef RESOURCEPATH_H
#define RESOURCEPATH_H

#include <string>
#include <filesystem>

class ResourcePath {
public:
    ResourcePath(const std::string& baseDir = "../");
    static std::string get(const std::string& relativePath);
    static std::string getBaseDir();
    static bool exists(const std::string& relativePath);

private:
    static std::string baseDir_;
    static std::string normalizePath(const std::string& path);
};

#endif // RESOURCEPATH_H