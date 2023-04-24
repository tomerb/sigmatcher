#define BOOST_TEST_MODULE sigmatcher_size_comparison
#include <boost/test/included/unit_test.hpp>

#include "test_common.h"

static unique_ptr<SignatureMatcher> PopulateDbTimed(SignatureMatcherType type)
{
    auto sig_matcher = SignatureMatcherFactory::Create(type);

    auto start = chrono::high_resolution_clock::now();

    for (const auto& file : directory_iterator(MALICIOUS_DATASET_DIR))
    {
        //cout << file.path() << endl;
        sig_matcher->Add(file.path());
    }

    auto end = chrono::high_resolution_clock::now();
    double elapsed_time_sec = chrono::duration<double>(end-start).count();
    cout << "Total add time: " << elapsed_time_sec << " seconds." << endl;

    return sig_matcher;
}

BOOST_AUTO_TEST_CASE(serialize_crc32)
{
    auto sig_matcher = PopulateDbTimed(SignatureMatcherType::SMT_CRC32);

    BOOST_TEST(sig_matcher->Serialize(CRC32_FILENAME));
}

BOOST_AUTO_TEST_CASE(serialize_bloom_filter)
{
    auto sig_matcher = PopulateDbTimed(SignatureMatcherType::SMT_BLOOM_FILTER);

    BOOST_TEST(sig_matcher->Serialize(BF_FILENAME));
}

BOOST_AUTO_TEST_CASE(comapre_size)
{
    auto crc_size = filesystem::file_size(CRC32_FILENAME);
    auto bf_size = filesystem::file_size(BF_FILENAME);
    cout << "CRC32 DB file size: " << crc_size << " bytes" << endl;
    cout << "Bloom filter DB file size: " << bf_size << " bytes" << endl;
    BOOST_TEST(crc_size > bf_size);
}
