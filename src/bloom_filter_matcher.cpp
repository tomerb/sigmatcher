#include "bloom_filter_matcher.h"

using namespace std;

namespace sigmatcher
{

void BloomFilterMatcher::Add(const string &file_path)
{
}

bool BloomFilterMatcher::Check(const string &file_path) const
{
    return false;
}

bool BloomFilterMatcher::Serialize(const string &file_path) const
{
    return false;
}

bool BloomFilterMatcher::Deserialize(const string &file_path)
{
    return false;
}

}
