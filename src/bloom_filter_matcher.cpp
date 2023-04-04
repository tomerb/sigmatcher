#include "bloom_filter_matcher.h"

using namespace std;

namespace sigmatcher {

void BloomFilterMatcher::Add(const string &file_path, uint64_t &duration_ms) {
}

bool BloomFilterMatcher::FoundMatch(const Signature &sig, uint64_t &duration_ms) const {
    return false;
}

bool BloomFilterMatcher::Serialize(const string &file_path) const {
    return false;
}

bool BloomFilterMatcher::Deserialize(const string &file_path) {
    return false;
}

}
