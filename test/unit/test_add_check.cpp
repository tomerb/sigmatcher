#define BOOST_TEST_MODULE sigmatcher_bf_tests
#include <boost/test/included/unit_test.hpp>

#include "../../src/signature_matcher_factory.h"

using namespace sigmatcher;
using namespace std;

const string SMALL_FILE = "samples/small.dat";
const string MEDIUM_FILE = "samples/medium.dat";
const string LARGE_FILE = "samples/large.dat";

static bool AddAndCheck(SignatureMatcherType matcher_type,
                        const string &file_to_add,
                        const string &file_to_check)
{
    auto sig_matcher =
        SignatureMatcherFactory::Create(matcher_type);
    sig_matcher->Add(file_to_add);
    return sig_matcher->Check(file_to_check);
}

BOOST_AUTO_TEST_CASE(crc32_add_and_check)
{
    BOOST_TEST(AddAndCheck(SignatureMatcherType::SMT_CRC32, SMALL_FILE, SMALL_FILE));
    BOOST_TEST(AddAndCheck(SignatureMatcherType::SMT_CRC32, MEDIUM_FILE, MEDIUM_FILE));
    BOOST_TEST(AddAndCheck(SignatureMatcherType::SMT_CRC32, LARGE_FILE, LARGE_FILE));
    BOOST_TEST(!AddAndCheck(SignatureMatcherType::SMT_CRC32, SMALL_FILE, LARGE_FILE));
    BOOST_TEST(!AddAndCheck(SignatureMatcherType::SMT_CRC32, MEDIUM_FILE, LARGE_FILE));
}

BOOST_AUTO_TEST_CASE(bf_add_and_check)
{
    BOOST_TEST(AddAndCheck(SignatureMatcherType::SMT_BLOOM_FILTER, SMALL_FILE, SMALL_FILE));
    BOOST_TEST(AddAndCheck(SignatureMatcherType::SMT_BLOOM_FILTER, MEDIUM_FILE, MEDIUM_FILE));
    BOOST_TEST(AddAndCheck(SignatureMatcherType::SMT_BLOOM_FILTER, LARGE_FILE, LARGE_FILE));
    BOOST_TEST(!AddAndCheck(SignatureMatcherType::SMT_BLOOM_FILTER, SMALL_FILE, LARGE_FILE));
    BOOST_TEST(!AddAndCheck(SignatureMatcherType::SMT_BLOOM_FILTER, MEDIUM_FILE, LARGE_FILE));
}
