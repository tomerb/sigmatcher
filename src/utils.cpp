#include "utils.h"

#include <boost/crc.hpp>
#include <openssl/evp.h>
#include <iostream>

using namespace std;

namespace sigmatcher
{

uint32_t Utils::Crc32Checksum(const TBuffer &buf)
{
    boost::crc_32_type crc;
    crc.process_bytes(buf.data(), buf.size());
    return crc.checksum();
}

static void sha256_hash_string (unsigned char hash[32],
                                unsigned char outputBuffer[65])
{
    int i = 0;

    for(i = 0; i < 32; i++)
    {
        sprintf((char*)outputBuffer + (i * 2), "%02x", hash[i]);
    }

    outputBuffer[64] = 0;
}

static bool UpdateDigest(EVP_MD_CTX *mdctx, const string &file_path)
{
    FILE *file = fopen(file_path.c_str(), "rb");
    if(!file)
    {
        cout << "Unable to open file " << file_path << endl;
        return false;
    }

    const int bufSize = 32*1024;
    unsigned char *buffer = (unsigned char*)malloc(bufSize);
    int bytesRead = 0;
    if(!buffer) return false;
    bool success = true;
    while((bytesRead = fread(buffer, 1, bufSize, file)))
    {
        if(1 != EVP_DigestUpdate(mdctx, buffer, bytesRead))
        {
            success = false;
            break;
        }
    }
    fclose(file);
    free(buffer);
    return success;
}

// Copied from https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c/10632725
bool Utils::Sha256File(const string &file_path, unsigned char outputBuffer[65])
{
    EVP_MD_CTX *mdctx;

    if((mdctx = EVP_MD_CTX_new()) == NULL)
    {
        cout << "Failed in EVP_MD_CTX_new()" << endl;
        return false;
    }

    if(1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL))
    {
        cout << "Failed in EVP_DigestInit_ex" << endl;
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    if (!UpdateDigest(mdctx, file_path))
    {
        cout << "Failed in UpdateDigest" << endl;
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    unsigned char hash[32];
    unsigned int hash_len;
    if(1 != EVP_DigestFinal_ex(mdctx, hash, &hash_len))
    {
        cout << "Failed in EVP_DigestFinal_ex" << endl;
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    EVP_MD_CTX_free(mdctx);

    sha256_hash_string(hash, outputBuffer);

    return true;
}

}
