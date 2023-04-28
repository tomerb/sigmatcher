#pragma once

#include <string>

namespace sigmatcher {

class SignatureMatcher {
 public:
    virtual ~SignatureMatcher() = default;

    /**
     * Add a file to the internal database of this signature matcher.
     * Each concrete implementation may choose its own way of extracting a signature for a given
     * file, and its own way of storing that signature.
     * @param file_path the path to the file that its signature will be added
     */
    virtual void Add(const std::string &file_path) = 0;

    /**
     * Check if a file was previously added to the internal database of this signature matcher.
     * @param file_path the path to the file that its signature needs to be checked
     * @return true if the file was previously added, false otherwise
     */
    virtual bool Check(const std::string &file_path) const = 0;

    /**
     * Store the state of this signature matcher into persistent storage.
     * @param file_path the path of the file to store
     * @return true if the operation succeeded, false otherwise
     */
    virtual bool Serialize(const std::string &file_path) const = 0;

    /**
     * Load a state that was previously saved using Serialize() into this signature matcher.
     * If this matcher already has stored data, it will be removed, and replaced by the new data.
     * @param file_path the path of the file to load from
     * @return true if the operation succeeded, false otherwise
     */
    virtual bool Deserialize(const std::string &file_path) = 0;
};

}
