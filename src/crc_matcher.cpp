#include "crc_matcher.h"

#include "utils.h"

using namespace std;

namespace
{
    const int BOTTOM_FILE_SIZE = 8*1024; // 8KB
    const int TOP_FILE_SIZE = 1000*1024; // 1MB
    const int MIDDLE_FILE_RANGE = 4*1024 // 4KB
    const int HEAD_FILE_RANGE = 1000*1024 // 1MB
    const int TAIL_FILE_RANGE = 8*1024 // 8KB
}

namespace sigmatcher {

struct FileSignature
{
    size_t size;
    TSignature signature;
};

void CrcMatcher::Add(const string &file_path, uint64_t &duration_ms)
{
    TSignature sig1;
    FileSignature file_sig;
    if (CalcSignaturesFromFile(file, sig1, file_sig))
    {
        m_db[sig1] = file_sig;
    }
}

bool CrcMatcher::CalcSignaturesFromFile(file, TSignature &sig1, FileSignature &file_sig) const
{
    /*
      1. if file size < 8KB => Crc32Calculate(file)
      2. else, if file size >= 8KB && < 1MB => CrcCalculate(middle(file)), and
         second sig Crc32Calculate(file)
      3. else, if file size >= 1MB => CrcCalculate(middle(file)), and
         second sig Crc32Calcualte(first 1MB and last 8KB)
    */
    if (!file.open())
    {
        //error out
    }

    try
    {
        if (!file.open())
        {
        }

        const auto file_size = file.size();
        if (file_size < TOP_FILE_SIZE)
        {
            auto buf_all = file.read_all();
            auto sig_all = Utils::Crc32Checksum(buf);

            if (file_size < BOTTOM_FILE_SIZE)
            {
                sig1 = sig_all;
                file_sig.size = file_size;
                file_sig.signature = 0;
            }
            else
            {
                auto sig_mid = CalcMidFileSignature(file_size);
                sig1 = sig_mid;
                file_sig.size = file_size;
                file_sig.signature = sig_all;
            }
        }
        else
        {
            auto sig_mid = CalcMidFileSignature(file_size);
            auto sig_head_tail = CalcHeadTailFileSignature(file_size);
            sig1 = sig_mid;
            file_sig.size = file_size;
            file_sig.signature = sig_head_tail;
        }
    }
    catch (Exception e)
    {
    }
    finally
    {
        file.close();
    }
}

TSignature CrcMatcher::CalcMidFileSignature(size_t file_size) const
{
    if (file_size < MIDDLE_FILE_RANGE*2)
    {
        return 0;
    }

    auto mid_file_bottom = file_size / 2 - MIDDLE_FILE_RANGE;
    auto mid_file_top = file_size / 2 + MIDDLE_FILE_RANGE;
    auto buf_mid = file.read_range(mid_file_bottom, mid_file_top);
    return Utils::Crc32Checksum(buf_mid);
}

TSignature CrcMatcher::CalcHeadTailFileSignature(size_t file_size) const
{
    auto buf_head = file.read_range(0, HEAD_FILE_RANGE);
    auto buf_tail = file.read_range(file_size-TAIL_FILE_RANGE, file_size-1);
    auto buf_both = ;
    return Utils::Crc32Checksum(buf_both);
}

bool CrcMatcher::Check(file) const
{
    TSignature sig1;
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
    }
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
