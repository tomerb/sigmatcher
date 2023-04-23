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
    static bool Sha256File(const std::string &file_path,
                           unsigned char hash[SHA256_SIZE_BYTES]);
    static std::string Sha256ToString(const unsigned char hash[SHA256_SIZE_BYTES]);
    static TSignature Murmur3(const unsigned char *buf, size_t buf_len);
 private:
    Utils() {}
};

}
