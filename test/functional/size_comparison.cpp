#define BOOST_TEST_MODULE sigmatcher_serializations
#include <boost/test/included/unit_test.hpp>

#include <filesystem>

#include "../../src/signature_matcher_factory.h"

const std::string BENIGN_DATASET_DIR = "../../tools/benign_dataset";
const std::string MALICIOUS_DATASET_DIR = "../../tools/malicious_dataset";

using namespace sigmatcher;
using directory_iterator = std::filesystem::recursive_directory_iterator;

BOOST_AUTO_TEST_CASE(serialize_crc32)
{
    auto sig_matcher =
        SignatureMatcherFactory::Create(SignatureMatcherType::SMT_CRC32);
    for (const auto& file : directory_iterator(MALICIOUS_DATASET_DIR))
    {
        std::cout << file.path() << std::endl;
        sig_matcher->Add(file.path());
    }
    BOOST_TEST(sig_matcher->Serialize("./crc32.db"));
}

BOOST_AUTO_TEST_CASE(deserialize_crc32)
{
    auto sig_matcher =
        SignatureMatcherFactory::Create(SignatureMatcherType::SMT_CRC32);

    for (const auto& file : directory_iterator(MALICIOUS_DATASET_DIR))
    {
        std::cout << file.path() << std::endl;
        BOOST_TEST(!sig_matcher->Check(file.path()));
    }

    BOOST_TEST(sig_matcher->Deserialize("./crc32.db"));

    for (const auto& file : directory_iterator(MALICIOUS_DATASET_DIR))
    {
        std::cout << file.path() << std::endl;
        BOOST_TEST(sig_matcher->Check(file.path()));
    }

    for (const auto& file : directory_iterator(BENIGN_DATASET_DIR))
    {
        std::cout << file.path() << std::endl;
        BOOST_TEST(!sig_matcher->Check(file.path()));
    }
}
