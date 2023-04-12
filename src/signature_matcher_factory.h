#pragma once

#include "crc_matcher.h"

namespace sigmatcher
{

enum class SignatureMatcherType { SMT_CRC32, SMT_BLOOM_FILTER };

class SignatureMatcherFactory
{
 public:
    static unique_ptr<SignatureMatcher> Create(SignatureMatcherType type)
    {
        switch (type)
        {
        case SMT_CRC32:
            return make_unique<CrcMatcher>();
        default:
            return nullptr;
        }
    }
 private:
    SignatureMatcherFactory() {}
};

}
