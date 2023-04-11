#include "crc_matcher.h"

#include "utils.h"

using namespace std;

namespace
{
    const int BOTTOM_FILE_SIZE = 8*1024; // 8KB
    const int TOP_FILE_SIZE = 1000*1024; // 1MB
}

namespace sigmatcher {

void CrcMatcher::Add(const string &file_path, uint64_t &duration_ms)
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
        if (file_size < BOTTOM_FILE_SIZE)
        {
            auto buf = file.read_all();
            auto sig = Utils::Crc32Checksum();
            m_db[sig] = 0;
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

bool CrcMatcher::FoundMatch(const Signature &sig, uint64_t &duration_ms) const
{
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
