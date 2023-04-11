#pragma once

#include <cstdint> /* for uint32_t */

namespace sigmatcher
{

class Utils
{
 public:
    typedef uint32_t TSignature;

    static TSignature_t Crc32Checksum(uint32_t *data, std::size_t data_len);

 private:
    Utils() {}
};

}
