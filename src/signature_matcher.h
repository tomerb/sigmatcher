#pragma once

#include <string>

#include "common_defs.h"

namespace sigmatcher {

class SignatureMatcher {
 public:
    virtual ~SignatureMatcher() {}
    virtual void Add(const std::string &file_path) = 0;
    virtual bool Check(TSignature sig) const = 0;
    virtual bool Serialize(const std::string &file_path) const = 0;
    virtual bool Deserialize(const std::string &file_path) = 0;
};

}
