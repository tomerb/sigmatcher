#include "utils.h"

#include <boost/crc.hpp>

using namespace std;

namespace sigmatcher
{

uint32_t Utils::Crc32Checksum(uint32_t *data, size_t data_len)
{
    boost::crc_32_type crc;
    crc.process_bytes(data, data_len);
    return crc.checksum();
}

}
