#pragma once

#include <cstdint> /* for uint32_t */
#include <string>

#include "common_defs.h"

namespace sigmatcher
{

class Utils
{
 public:
    static uint32_t Crc32Checksum(const TBuffer &buf);
    static bool Sha256File(std::string file_path, unsigned char outputBuffer[65]);
 private:
    Utils() {}
};

}
