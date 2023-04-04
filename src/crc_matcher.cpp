#include "crc_matcher.h"

using namespace std;

namespace sigmatcher {

void CrcMatcher::Add(const string &file_path, uint64_t &duration_ms) {
}

bool CrcMatcher::FoundMatch(const Signature &sig, uint64_t &duration_ms) const {
    return false;
}

bool CrcMatcher::Serialize(const string &file_path) const {
    return false;
}

bool CrcMatcher::Deserialize(const string &file_path) {
    return false;
}

}
