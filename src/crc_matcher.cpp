#include "crc_matcher.h"

#include <fstream>
#include <iostream>
#include <cstring>

#include "utils.h"

using namespace std;

namespace
{
    const int BOTTOM_FILE_SIZE = 8*1024; // 8KB
    const int TOP_FILE_SIZE = 1000*1024; // 1MB
    const int MIDDLE_FILE_RANGE = 8*1024; // 8KB
    const int HEAD_FILE_RANGE = 1000*1024; // 1MB
    const int TAIL_FILE_RANGE = 8*1024; // 8KB
}

namespace sigmatcher {

static TSignature CalcMidFileSignature(ifstream &file, size_t file_size)
{
    if (file_size < MIDDLE_FILE_RANGE*2)
    {
        return 0;
    }

    auto mid_file_start = (file_size / 2) - (MIDDLE_FILE_RANGE / 2);
    file.seekg(mid_file_start);
    vector<byte> buf(MIDDLE_FILE_RANGE);
    if(!file.read((char*)buf.data(), buf.size()))
    {
        cout << strerror(errno) << endl;
        return false;
    }
    return Utils::Crc32Checksum(buf);
}

static TSignature CalcHeadTailFileSignature(ifstream &file, size_t file_size)
{
    /*auto buf_head = file.read_range(0, HEAD_FILE_RANGE);
    auto buf_tail = file.read_range(file_size-TAIL_FILE_RANGE, file_size-1);
    auto buf_both = ;
    return Utils::Crc32Checksum(buf_both);*/
    return 0;
}

bool CalcSignaturesFromFile(const string &file_path,
                            TSignature &sig1,
                            TSignature &sig2)
{
    /*
      1. if file size < 8KB => Crc32Calculate(file)
      2. else, if file size >= 8KB && < 1MB => CrcCalculate(middle(file)), and
         second sig Crc32Calculate(file)
      3. else, if file size >= 1MB => CrcCalculate(middle(file)), and
         second sig Crc32Calcualte(first 1MB and last 8KB)
    */

    ifstream file(file_path, ios::in | ios::binary);
    if (!file)
    {
        cout << file_path << ": " << strerror(errno) << endl;
        return false;
    }

    auto end = file.tellg();
    file.seekg(0, std::ios::beg);
    auto file_size = std::size_t(end - file.tellg());

    if (file_size == 0)
    {
        cout << "Illegal file size" << endl;
        return false;
    }

    if (file_size < TOP_FILE_SIZE)
    {
        vector<byte> buf(file_size);
        if(!file.read((char*)buf.data(), buf.size()))
        {
            cout << file_path << ": " << strerror(errno) << endl;
            return false;
        }

        auto sig_all = Utils::Crc32Checksum(buf);

        if (file_size < BOTTOM_FILE_SIZE)
        {
            sig1 = sig_all;
            sig2 = 0;
        }
        else
        {
            auto sig_mid = CalcMidFileSignature(file, file_size);
            sig1 = sig_mid;
            sig2 = sig_all;
        }
    }
    else
    {
        auto sig_mid = CalcMidFileSignature(file, file_size);
        auto sig_head_tail = CalcHeadTailFileSignature(file, file_size);
        sig1 = sig_mid;
        sig2 = sig_head_tail;
    }

    return true;
}

void CrcMatcher::Add(const string &file_path)
{
    TSignature sig1, sig2;
    if (CalcSignaturesFromFile(file_path, sig1, sig2))
    {
        m_db[sig1] = sig2;
    }
}

bool CrcMatcher::Check(TSignature sig) const
{
    /*TSignature sig1;
    FileSignature file_sig;
    if (CalcSignaturesFromFile(file, sig1, file_sig))
    {
        auto found = m_db.find(sig1);
        if (!found)
        {
            return false;
        }

        if (found == file_sig)
        {
            return true;
        }
        }*/
    return false;
}

bool CrcMatcher::Serialize(const string &file_path) const
{
    return false;
}

bool CrcMatcher::Deserialize(const string &file_path)
{
    return false;
}

}
