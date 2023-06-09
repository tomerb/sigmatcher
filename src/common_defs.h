#pragma once

#include <vector>

namespace sigmatcher
{
    typedef uint32_t TSignature;
    typedef std::vector<std::byte> TBuffer;

    const int SHA256_SIZE_BYTES = 32;
}
