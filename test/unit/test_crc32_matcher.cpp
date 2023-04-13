#define BOOST_TEST_MODULE sigmatcher_crc32_tests
#include <boost/test/included/unit_test.hpp>

#include "../../src/signature_matcher_factory.h"

using namespace sigmatcher;

BOOST_AUTO_TEST_CASE(check_same_file)
{
    auto sig_matcher =
        SignatureMatcherFactory::Create(SignatureMatcherType::SMT_CRC32);
    sig_matcher->Add("mal_test.dat");
    BOOST_TEST(sig_matcher->Check("mal_test.dat"));
}

BOOST_AUTO_TEST_CASE(check_different__file)
{
    auto sig_matcher =
        SignatureMatcherFactory::Create(SignatureMatcherType::SMT_CRC32);
    sig_matcher->Add("mal_test.dat");
    BOOST_TEST(!sig_matcher->Check("ben_test.dat"));
}
