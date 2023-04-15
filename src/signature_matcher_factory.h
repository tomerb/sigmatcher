#pragma once

#include <memory>

#include "crc_matcher.h"
#include "bloom_filter_matcher.h"

namespace sigmatcher
{

enum class SignatureMatcherType { SMT_CRC32, SMT_BLOOM_FILTER };

class SignatureMatcherFactory
{
 public:
    static std::unique_ptr<SignatureMatcher> Create(SignatureMatcherType type)
    {
        switch (type)
        {
        case SignatureMatcherType::SMT_CRC32:
            return std::make_unique<CrcMatcher>();
        case SignatureMatcherType::SMT_BLOOM_FILTER:
            return std::make_unique<BloomFilterMatcher>();
        default:
            return nullptr;
        }
    }
 private:
    SignatureMatcherFactory() {}
};

}
