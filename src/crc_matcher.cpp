#include "crc_matcher.h"

#include <fstream>
#include <iostream>
#include <iomanip> // for setprecision()
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
    vector<byte> buf(HEAD_FILE_RANGE + TAIL_FILE_RANGE);

    file.seekg(0, std::ios::beg);
    if(!file.read((char*)buf.data(), HEAD_FILE_RANGE))
    {
        cout << strerror(errno) << endl;
        return false;
    }

    file.seekg(file_size - TAIL_FILE_RANGE);
    if(!file.read((char*)buf.data(), TAIL_FILE_RANGE))
    {
        cout << strerror(errno) << endl;
        return false;
    }

    return Utils::Crc32Checksum(buf);
}

void PrintSignatures(TSignature sig1, TSignature sig2)
{
    cout << std::hex << "sig1=0x" << sig1 << ", sig2=0x" << sig2 << endl;
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

    ifstream file(file_path, ios::in | ios::binary | ios::ate);
    if (!file || file.fail())
    {
        cout << file_path << ": " << strerror(errno) << endl;
        return false;
    }

    auto end = file.tellg();
    file.seekg(0, std::ios::beg);
    auto file_size = std::size_t(end - file.tellg());

    if (file_size == 0)
    {
        cout << file_path << ": " << "Illegal file size (end=" << end << ")" << endl;
        return false;
    }

    //cout << "File size is " << setprecision(3) << static_cast<float>(file_size)/1000/1024 << " MB. ";

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
            //cout << "Generating a single signature: ";
            //PrintSignatures(sig_all, 0);
            sig1 = sig_all;
            sig2 = 0;
        }
        else
        {
            //cout << "Generating two signature with full content: ";
            auto sig_mid = CalcMidFileSignature(file, file_size);
            //PrintSignatures(sig_mid, sig_all);
            sig1 = sig_mid;
            sig2 = sig_all;
        }
    }
    else
    {
        //cout << "Generating two signature with partial content: ";
        auto sig_mid = CalcMidFileSignature(file, file_size);
        auto sig_head_tail = CalcHeadTailFileSignature(file, file_size);
        //PrintSignatures(sig_mid, sig_head_tail);
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

bool CrcMatcher::Check(const std::string &file_path) const
{
    TSignature sig1, sig2;
    if (CalcSignaturesFromFile(file_path, sig1, sig2))
    {
        if (auto found = m_db.find(sig1); found != m_db.end())
        {
            if (found->second == sig2)
            {
                //cout << "Check: found signature for " << file_path << endl;
                return true;
            }
        }
    }

    //cout << "Check: could not find signature for " << file_path << endl;
    return false;
}

bool CrcMatcher::Serialize(const string &file_path) const
{
    ofstream file(file_path, ios::out);
    for (auto sig : m_db)
    {
        file << hex << sig.first << " " << sig.second << endl;
    }
    file.close();
    return true;
}

bool CrcMatcher::Deserialize(const string &file_path)
{
    ifstream file(file_path, ios::in);
    if (!file)
    {
        cout << "Deserialize: file " << file_path << " not found" << endl;
        return false;
    }

    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        uint32_t sig1, sig2;
        if (!(iss >> hex >> sig1 >> sig2))
        {
            cout << "Deserialize: failed reading from file " << file_path << endl;
            m_db.clear();
            return false;
        }

        m_db[sig1] = sig2;
    }

    return true;
}

}
