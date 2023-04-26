#define BOOST_TEST_MODULE sigmatcher_response_time
#include <boost/test/included/unit_test.hpp>

#include "test_common.h"

static bool CheckDataset(const unique_ptr<SignatureMatcher> &sig_matcher,
                         const string &dataset_dir,
                         bool expected)
{
    BOOST_REQUIRE(filesystem::exists(dataset_dir));

    int total_checks = 0;
    auto total_time = chrono::nanoseconds::zero();
    bool had_failures = false;

    for (const auto &file : directory_iterator(dataset_dir))
    {
        auto start = chrono::high_resolution_clock::now();

        bool result = sig_matcher->Check(file.path());

        auto end = chrono::high_resolution_clock::now();

        total_time += (end - start);
        total_checks++;

        if (result != expected)
        {
            had_failures = true;
        }
    }

    BOOST_REQUIRE(total_checks > 0);

    chrono::duration<double> avg_time = total_time / total_checks;

    cout << "Conducted " << total_checks << " tests with total execution time of " <<
        chrono::duration<double>(total_time).count() << " seconds, and average execution time of " <<
        avg_time.count() << " seconds" << endl;

    return had_failures;
}

static void LoadAndCheckDataset(const string &dataset_dir, bool expected)
{
    {
        auto sig_matcher =
            LoadDbFile(SignatureMatcherType::SMT_CRC32, CRC32_FILENAME);
        cout << "*** testing CRC32 matcher response time for files under " <<
            dataset_dir << " ***" << endl;
        BOOST_TEST(CheckDataset(sig_matcher, dataset_dir, expected));
    }

    {
        auto sig_matcher =
            LoadDbFile(SignatureMatcherType::SMT_BLOOM_FILTER, BF_FILENAME);
        cout << "*** testing bloom filter matcher response time for files under " <<
            dataset_dir << " ***" << endl;
        BOOST_TEST(CheckDataset(sig_matcher, dataset_dir, expected));
    }
}

BOOST_AUTO_TEST_CASE(test_malicious_dataset)
{
    LoadAndCheckDataset(MALICIOUS_DATASET_DIR, true);
}

BOOST_AUTO_TEST_CASE(test_benign_dataset)
{
    LoadAndCheckDataset(BENIGN_DATASET_DIR, false);
}
