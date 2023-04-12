#include "../../src/signature_matcher_factory.h"

using namespace sigmatcher;

int main()
{
    auto sig_matcher = SignatureMatcherFactory::Create(SMT_CRC32);
    sig_matcher.Add(&file_handler);
    //    sig_matcher.Check(file_handler.Get
    return -1;
}
