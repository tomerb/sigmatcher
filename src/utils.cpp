#include "utils.h"

#include <boost/crc.hpp>

using namespace std;

namespace sigmatcher
{

uint32_t Utils::Crc32Checksum(const TBuffer &buf)
{
    boost::crc_32_type crc;
    crc.process_bytes(buf.data(), buf.size());
    return crc.checksum();
}

}
