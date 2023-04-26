#include "bloom_filter_matcher.h"

#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include "utils.h"

using namespace std;

namespace sigmatcher
{

// Genarated randomly using the following command:
// for i in {1..12}; do hexdump -vn4 -e'4/4 "%08X" 1 "\n"' /dev/urandom; done
static const uint32_t murmur3_seeds[] = {
  0xFD4953DF, 0x3793686A, 0x153DB45C, 0x8646BA78, 0xE8853AD9, 0x274C3F30,
  0xD8F0C72B, 0xF9C63AE2, 0x290FF7C3, 0x7315BB2D, 0x02475286, 0xF4993085
};

BloomFilterMatcher::BloomFilterMatcher(int number_of_items, double fp_probability) :
    m_n(number_of_items),
    m_p(fp_probability),
    m_m(ceil((m_n * log(m_p)) / log(1 / pow(2, log(2))))),
    m_k(round((m_m / m_n) * log(2))),
    m_bitset(m_m, false)
{
    cout << "Creating a bloom filter signature matcher with n=" << m_n <<
        ", p=" << m_p << ", m=" << m_m << ", k=" << m_k << endl;
}

bool BloomFilterMatcher::CalcBitsPosition(const string &file_path,
                                          vector<size_t> &positions) const
{
    if (m_k > sizeof(murmur3_seeds))
    {
      cout << "Not enough seeds to calculate bits position" << endl;
      return false;
    }

    cout << "CalcBitsPosition() for: " << file_path << endl;

    static unsigned char hash[SHA256_SIZE_BYTES];

    if (Utils::Sha256File(file_path, hash))
    {
        //auto str_hash = Utils::Sha256ToString(hash);
        //cout << str_hash << endl;

        for (size_t i = 0; i < m_k; i++)
        {
            auto murmur = Utils::Murmur3(hash, sizeof(hash), murmur3_seeds[i]);
            size_t pos = murmur % m_m;
            cout << "pos[" << i << "]: " << pos << endl;
            positions.push_back(pos);
        }

        return true;
    }

    return false;
}

void BloomFilterMatcher::Add(const string &file_path)
{
    vector<size_t> positions;
    if (CalcBitsPosition(file_path, positions))
    {
        for (auto &p : positions)
        {
            m_bitset[p] = true;
        }
    }
}

bool BloomFilterMatcher::Check(const string &file_path) const
{
    vector<size_t> positions;
    if (CalcBitsPosition(file_path, positions))
    {
        for (auto p : positions)
        {
            cout << p << endl;
            if (!m_bitset[p]) return false;
        }
    }

    cout << "Check passed for " << file_path << endl;

    return true;
}

bool BloomFilterMatcher::Serialize(const string &file_path) const
{
    ofstream file(file_path, ios::out);
    if (!file)
    {
        cout << "Serialize: file " << file_path << " cannot be opened for writing" << endl;
        return false;
    }

    uint32_t to_write = 0;
    int offset = sizeof(to_write)-1;
    for (bool b : m_bitset)
    {
        to_write |= b << offset--;
        if (!offset)
        {
            file << to_write;
            offset = sizeof(to_write)-1;
            to_write = 0;
        }
    }

    file.close();

    return true;
}

bool BloomFilterMatcher::Deserialize(const string &file_path)
{
    ifstream file(file_path, ios::in);
    if (!file)
    {
        cout << "Deserialize: file " << file_path << " not found" << endl;
        return false;
    }

    m_bitset.clear();

    uint32_t to_read;
    int offset = sizeof(to_read);
    while (file >> to_read)
    {
        m_bitset.push_back(to_read >> offset--);
        if (!offset)
        {
            offset = sizeof(to_read);
        }
    }

    file.close();

    return true;
}

}
