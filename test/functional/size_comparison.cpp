#define BOOST_TEST_MODULE sigmatcher_serializations
#include <boost/test/included/unit_test.hpp>

#include <filesystem>
#include <chrono>

#include "../../src/signature_matcher_factory.h"

using namespace sigmatcher;
using namespace std;
using directory_iterator = std::filesystem::recursive_directory_iterator;

const string BENIGN_DATASET_DIR = "../../tools/benign_dataset";
const string MALICIOUS_DATASET_DIR = "../../tools/malicious_dataset";

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

    BOOST_TEST(sig_matcher->Serialize("./crc32.db"));
}

BOOST_AUTO_TEST_CASE(serialize_bloom_filter)
{
    auto sig_matcher = PopulateDbTimed(SignatureMatcherType::SMT_BLOOM_FILTER);

    BOOST_TEST(sig_matcher->Serialize("./bloom.db"));
}

/*
BOOST_AUTO_TEST_CASE(deserialize_crc32)
{
    auto sig_matcher =
        SignatureMatcherFactory::Create(SignatureMatcherType::SMT_CRC32);

    for (const auto& file : directory_iterator(MALICIOUS_DATASET_DIR))
    {
        //cout << file.path() << endl;
        BOOST_TEST(!sig_matcher->Check(file.path()));
    }

    BOOST_TEST(sig_matcher->Deserialize("./crc32.db"));

    for (const auto& file : directory_iterator(MALICIOUS_DATASET_DIR))
    {
        //cout << file.path() << endl;
        BOOST_TEST(sig_matcher->Check(file.path()));
    }

    for (const auto& file : directory_iterator(BENIGN_DATASET_DIR))
    {
        //cout << file.path() << endl;
        BOOST_TEST(!sig_matcher->Check(file.path()));
    }
}
*/
