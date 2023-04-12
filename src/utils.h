#pragma once

#include <cstdint> /* for uint32_t */

#include "common_defs.h"

namespace sigmatcher
{

class Utils
{
 public:
    static uint32_t Crc32Checksum(const TBuffer &buf);

 private:
    Utils() {}
};

}
