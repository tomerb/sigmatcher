#define BOOST_TEST_MODULE sigmatcher_accuracy_perf
#include <boost/test/included/unit_test.hpp>

#include "test_common.h"

BOOST_AUTO_TEST_CASE(deserialize_crc32)
{
    auto sig_matcher =
        SignatureMatcherFactory::Create(SignatureMatcherType::SMT_CRC32);

    for (const auto& file : directory_iterator(MALICIOUS_DATASET_DIR))
    {
        //cout << file.path() << endl;
        BOOST_TEST(!sig_matcher->Check(file.path()));
    }

    BOOST_TEST(sig_matcher->Deserialize(CRC32_FILENAME));

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
