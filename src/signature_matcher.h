#pragma once

#include <string>

#include "common_defs.h"

namespace sigmatcher {

typedef uint32_t Signature;

class SignatureMatcher {
 public:
    virtual ~SignatureMatcher() {}
    virtual void Add(const std::string &file_path, uint64_t &duration_ms) = 0;
    virtual bool FoundMatch(const Signature &sig, uint64_t &duration_ms) const = 0;
    virtual bool Serialize(const std::string &file_path) const = 0;
    virtual bool Deserialize(const std::string &file_path) = 0;
};

}
