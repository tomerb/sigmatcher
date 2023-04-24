#pragma once

#include <filesystem>
#include <chrono>

#include "../../src/signature_matcher_factory.h"

using namespace sigmatcher;
using namespace std;

using directory_iterator = std::filesystem::recursive_directory_iterator;

const string BENIGN_DATASET_DIR = "../../tools/benign_dataset";
const string MALICIOUS_DATASET_DIR = "../../tools/malicious_dataset";

const string CRC32_FILENAME = "./crc32.db";
const string BF_FILENAME = "./bloom.db";
