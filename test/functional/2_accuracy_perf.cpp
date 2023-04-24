#define BOOST_TEST_MODULE sigmatcher_accuracy_perf
#include <boost/test/included/unit_test.hpp>

#include "test_common.h"

static unique_ptr<SignatureMatcher> LoadDbFile(SignatureMatcherType type,
                                               const string &filename)
{
    BOOST_REQUIRE(filesystem::exists(filename));

    auto sig_matcher = SignatureMatcherFactory::Create(type);
    BOOST_REQUIRE((sig_matcher != nullptr));

    BOOST_REQUIRE(sig_matcher->Deserialize(filename));

    return sig_matcher;
}

static bool CheckDataset(const unique_ptr<SignatureMatcher> &sig_matcher,
                         const string &dataset_dir,
                         bool exists)
{
    BOOST_REQUIRE(filesystem::exists(dataset_dir));

    bool has_files = false;
    int failed_checks = 0;
    for (const auto &file : directory_iterator(dataset_dir))
    {
        has_files = true;
        if (sig_matcher->Check(file.path()) != exists)
        {
            failed_checks++;
        }
    }

    BOOST_REQUIRE(has_files);

    cout << failed_checks << " tests failed" << endl;

    return failed_checks == 0;
}

static bool CheckAllDatasets(const unique_ptr<SignatureMatcher> &sig_matcher)
{
    return CheckDataset(sig_matcher, MALICIOUS_DATASET_DIR, true) &&
        CheckDataset(sig_matcher, BENIGN_DATASET_DIR, false);
}

BOOST_AUTO_TEST_CASE(test_signatures_crc32)
{
    auto sig_matcher =
        LoadDbFile(SignatureMatcherType::SMT_CRC32, CRC32_FILENAME);
    BOOST_TEST(CheckAllDatasets(sig_matcher));
}

BOOST_AUTO_TEST_CASE(test_signatures_bf)
{
    auto sig_matcher =
        LoadDbFile(SignatureMatcherType::SMT_BLOOM_FILTER, BF_FILENAME);
    BOOST_TEST(CheckAllDatasets(sig_matcher));
}
