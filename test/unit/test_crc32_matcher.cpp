#include "../../src/signature_matcher_factory.h"

using namespace sigmatcher;

int main()
{
    auto sig_matcher = SignatureMatcherFactory::Create(SignatureMatcherType::SMT_CRC32);
    sig_matcher->Add("./mal_test.dat");
    //Utils::Crc32Checksum();
    //    sig_matcher.Check(file_handler.Get
    return -1;
}
