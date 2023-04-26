#define BOOST_TEST_MODULE sigmatcher_unit_tests
#include <boost/test/included/unit_test.hpp>

#include "../../src/signature_matcher_factory.h"

using namespace sigmatcher;
using namespace std;

const string SMALL_FILE = "samples/small.dat";
const string MEDIUM_FILE = "samples/medium.dat";
const string LARGE_FILE = "samples/large.dat";
const string DUMMY_FILE = "samples/dummy.dat";

const string CRC32_FILENAME = "./crc32.db";
const string BF_FILENAME = "./bloom.db";
const string COMPARE_FILENAME = "./compare.db";

static bool AddAndCheck(SignatureMatcherType matcher_type,
                        const string &file_to_add,
                        const string &file_to_check)
{
    auto sig_matcher =
        SignatureMatcherFactory::Create(matcher_type);
    BOOST_REQUIRE((sig_matcher != nullptr));

    sig_matcher->Add(file_to_add);
    return sig_matcher->Check(file_to_check);
}

static void AddAndCheckSuite(SignatureMatcherType matcher_type)
{
    BOOST_TEST(AddAndCheck(matcher_type, SMALL_FILE, SMALL_FILE));
    BOOST_TEST(AddAndCheck(matcher_type, MEDIUM_FILE, MEDIUM_FILE));
    BOOST_TEST(AddAndCheck(matcher_type, LARGE_FILE, LARGE_FILE));
    BOOST_TEST(!AddAndCheck(matcher_type, SMALL_FILE, LARGE_FILE));
    BOOST_TEST(!AddAndCheck(matcher_type, MEDIUM_FILE, LARGE_FILE));
}

BOOST_AUTO_TEST_CASE(crc32_add_and_check)
{
    AddAndCheckSuite(SignatureMatcherType::SMT_CRC32);
}

BOOST_AUTO_TEST_CASE(bf_add_and_check)
{
    AddAndCheckSuite(SignatureMatcherType::SMT_BLOOM_FILTER);
}

static void CompareFiles(const string &filename1, const string &filename2)
{
    ifstream ifs1(filename1);
    ifstream ifs2(filename2);

    istream_iterator<char> b1(ifs1), e1;
    istream_iterator<char> b2(ifs2), e2;

    BOOST_CHECK_EQUAL_COLLECTIONS(b1, e1, b2, e2);
}

static void SerdeSuite(SignatureMatcherType matcher_type, const string &filename)
{
    auto sig_matcher_pre_serde = SignatureMatcherFactory::Create(matcher_type);
    BOOST_REQUIRE((sig_matcher_pre_serde != nullptr));

    sig_matcher_pre_serde->Add(SMALL_FILE);
    sig_matcher_pre_serde->Add(MEDIUM_FILE);
    sig_matcher_pre_serde->Add(LARGE_FILE);

    BOOST_TEST(sig_matcher_pre_serde->Serialize(filename));

    auto sig_matcher_post_serde = SignatureMatcherFactory::Create(matcher_type);
    BOOST_REQUIRE((sig_matcher_post_serde != nullptr));

    BOOST_TEST(sig_matcher_post_serde->Deserialize(filename));

    BOOST_TEST(sig_matcher_post_serde->Check(SMALL_FILE));
    BOOST_TEST(sig_matcher_post_serde->Check(MEDIUM_FILE));
    BOOST_TEST(sig_matcher_post_serde->Check(LARGE_FILE));
    BOOST_TEST(!sig_matcher_post_serde->Check(DUMMY_FILE));

    BOOST_TEST(sig_matcher_post_serde->Serialize(COMPARE_FILENAME));
    CompareFiles(filename, COMPARE_FILENAME);
}

BOOST_AUTO_TEST_CASE(crc32_serde)
{
    SerdeSuite(SignatureMatcherType::SMT_CRC32, CRC32_FILENAME);
}

BOOST_AUTO_TEST_CASE(bf_serde)
{
    SerdeSuite(SignatureMatcherType::SMT_BLOOM_FILTER, BF_FILENAME);
}
