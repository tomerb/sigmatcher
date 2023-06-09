#pragma once

#include "signature_matcher.h"

#include <vector>

namespace sigmatcher
{

class BloomFilterMatcher : public SignatureMatcher
{
 public:
    BloomFilterMatcher(int number_of_items, double fp_probability);
    virtual void Add(const std::string &file_path);
    virtual bool Check(const std::string &file_path) const;
    virtual bool Serialize(const std::string &file_path) const;
    virtual bool Deserialize(const std::string &file_path);
private:
    // Number of items in the filter
    size_t m_n;

    // Probability of false positives
    double m_p;

    // Number of bits in the filter
    uint32_t m_m;

    // Number of hash functions
    size_t m_k;

    std::vector<bool> m_bitset;

    bool CalcBitsPosition(const std::string &file_path,
                          std::vector<size_t> &positions) const;
};

}
