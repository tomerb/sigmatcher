#include "utils.h"

#include <boost/crc.hpp>
#include <openssl/sha.h>

using namespace std;

namespace sigmatcher
{

uint32_t Utils::Crc32Checksum(const TBuffer &buf)
{
    boost::crc_32_type crc;
    crc.process_bytes(buf.data(), buf.size());
    return crc.checksum();
}

static void sha256_hash_string (unsigned char hash[SHA256_DIGEST_LENGTH], unsigned char outputBuffer[65])
{
    int i = 0;

    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf((char*)outputBuffer + (i * 2), "%02x", hash[i]);
    }

    outputBuffer[64] = 0;
}

// Copied from https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c/10632725
bool Utils::Sha256File(string file_path, unsigned char outputBuffer[65])
{
    FILE *file = fopen(file_path.c_str(), "rb");
    if(!file) return false;

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    const int bufSize = 32768;
    unsigned char *buffer = (unsigned char*)malloc(bufSize);
    int bytesRead = 0;
    if(!buffer) return false;
    while((bytesRead = fread(buffer, 1, bufSize, file)))
    {
        SHA256_Update(&sha256, buffer, bytesRead);
    }
    SHA256_Final(hash, &sha256);

    sha256_hash_string(hash, outputBuffer);
    fclose(file);
    free(buffer);
    return true;
}

}
