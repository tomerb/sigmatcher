#pragma once

#include "./signature_matcher.h"

#include <map>

namespace sigmatcher {

class CrcMatcher : public SignatureMatcher {
 public:
    virtual void Add(const std::string &file_path, uint64_t &duration_ms);
    virtual bool FoundMatch(const Signature &sig, uint64_t &duration_ms) const;
    virtual bool Serialize(const std::string &file_path) const;
    virtual bool Deserialize(const std::string &file_path);
private:
    std::map<TSignature, TSignature> m_db;
};

}
