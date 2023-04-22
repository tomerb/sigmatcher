#define BOOST_TEST_MODULE sigmatcher_bf_tests
#include <boost/test/included/unit_test.hpp>

#include "../../src/signature_matcher_factory.h"

const std::string SMALL_FILE = "samples/small.dat";
const std::string MEDIUM_FILE = "samples/medium.dat";
const std::string LARGE_FILE = "samples/large.dat";

using namespace sigmatcher;

BOOST_AUTO_TEST_CASE(check_same_file)
{
    auto sig_matcher =
        SignatureMatcherFactory::Create(SignatureMatcherType::SMT_BLOOM_FILTER);
    sig_matcher->Add(SMALL_FILE);
    BOOST_TEST(sig_matcher->Check(SMALL_FILE));
}

BOOST_AUTO_TEST_CASE(check_different_file)
{
    auto sig_matcher =
        SignatureMatcherFactory::Create(SignatureMatcherType::SMT_BLOOM_FILTER);
    sig_matcher->Add(MEDIUM_FILE);
    BOOST_TEST(!sig_matcher->Check(LARGE_FILE));
}
