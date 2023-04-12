#pragma once

#include "./signature_matcher.h"

namespace sigmatcher
{

class BloomFilterMatcher : public SignatureMatcher
{
 public:
    virtual void Add(const std::string &file_path);
    virtual bool Check(TSignature sig) const;
    virtual bool Serialize(const std::string &filxe_path) const;
    virtual bool Deserialize(const std::string &file_path);
};

}
