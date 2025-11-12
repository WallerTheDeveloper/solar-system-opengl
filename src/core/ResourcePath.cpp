//
// Created by user on 11/12/2025.
//

#include "ResourcePath.h"
#include <iostream>

std::string ResourcePath::baseDir_ = "../";

ResourcePath::ResourcePath(const std::string &baseDir) {
    baseDir_ = normalizePath(baseDir);

    if (!baseDir_.empty() && baseDir_.back() != '/' && baseDir_.back() != '\\') {
        baseDir_ += '/';
    }

    std::cout << "ResourcePath initialized with base directory: " << baseDir_ << std::endl;
}

std::string ResourcePath::get(const std::string& relativePath) {
    if (relativePath.empty()) {
        return baseDir_;
    }

    // Combine base directory with relative path
    std::string fullPath = baseDir_ + relativePath;
    return normalizePath(fullPath);
}

std::string ResourcePath::getBaseDir() {
    return baseDir_;
}

bool ResourcePath::exists(const std::string& relativePath) {
    std::string fullPath = get(relativePath);
    return std::filesystem::exists(fullPath);
}

std::string ResourcePath::normalizePath(const std::string& path) {
    try {
        // Use filesystem to normalize the path
        std::filesystem::path p(path);
        p = p.lexically_normal();

        // Convert to string with forward slashes for consistency
        std::string normalized = p.string();

        // Replace backslashes with forward slashes for cross-platform consistency
        for (char& c : normalized) {
            if (c == '\\') {
                c = '/';
            }
        }

        return normalized;
    } catch (const std::exception& e) {
        std::cerr << "Error normalizing path '" << path << "': " << e.what() << std::endl;
        return path;
    }
}