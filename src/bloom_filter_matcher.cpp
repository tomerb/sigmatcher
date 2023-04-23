#include "bloom_filter_matcher.h"

#include <math.h>
#include <iostream>

#include "utils.h"

using namespace std;

namespace sigmatcher
{

BloomFilterMatcher::BloomFilterMatcher(int number_of_items, double fp_probability) :
    m_n(number_of_items),
    m_p(fp_probability),
    m_m(ceil((m_n * log(m_p)) / log(1 / pow(2, log(2))))),
    m_k(round((m_m / m_n) * log(2)))
{
    cout << "Creating a bloom filter signature matcher with n=" << m_n <<
        ", p=" << m_p << ", m=" << m_m << ", k=" << m_k << endl;

    m_bitset.reserve(m_m);
}

void BloomFilterMatcher::Add(const string &file_path)
{
    static unsigned char hash[SHA256_SIZE_BYTES];
    if (Utils::Sha256File(file_path, hash))
    {
        auto str_hash = Utils::Sha256ToString(hash);
        cout << str_hash << endl;
    }
}

bool BloomFilterMatcher::Check(const string &file_path) const
{
    cout << file_path << endl;
    return false;
}

bool BloomFilterMatcher::Serialize(const string &file_path) const
{
    cout << file_path << endl;
    return false;
}

bool BloomFilterMatcher::Deserialize(const string &file_path)
{
    cout << file_path << endl;
    return false;
}

}
