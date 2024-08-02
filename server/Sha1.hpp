#ifndef SHA1_H
#define SHA1_H

#include <cstdint>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>


constexpr size_t BLOCK_SIZE = 64;
//constexpr size_t HASH_SIZE = 20;

class SHA1
{
public:
    SHA1();
    void update(const std::string &s);
    void update(std::istream &is);
    std::string final();
    static std::string from_file(const std::string &filename);

private:
    void processBuffer();
    void processBlock(const uint8_t block[BLOCK_SIZE]);
    void padAndProcess();
    void reset();

    uint32_t digest[5];
    std::vector<uint8_t> buffer;
    uint64_t bitCount;
};

uint32_t rotateLeft(uint32_t value, unsigned int bits);
uint32_t byteToUInt32(const uint8_t bytes[4]);
void uint32ToByte(uint32_t value, uint8_t bytes[4]);

SHA1::SHA1()
{
    reset();
}

void SHA1::update(const std::string &s)
{
    std::istringstream stream(s);
    update(stream);
    bitCount += s.size() * 8;
}

void SHA1::update(std::istream &is)
{
    std::vector<uint8_t> inputBuffer(std::istreambuf_iterator<char>(is), {});
    buffer.insert(buffer.end(), inputBuffer.begin(), inputBuffer.end());
    bitCount += inputBuffer.size() * 8;
    processBuffer();
}

std::string SHA1::final()
{
    padAndProcess();

    std::ostringstream result;
    for (size_t i = 0; i < 5; ++i)
    {
        result << std::hex << std::setw(8) << std::setfill('0') << digest[i];
    }

    return result.str();
}

std::string SHA1::from_file(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    SHA1 sha1;
    sha1.update(file);
    return sha1.final();
}

void SHA1::processBuffer()
{
    while (buffer.size() >= BLOCK_SIZE)
    {
        processBlock(buffer.data());
        buffer.erase(buffer.begin(), buffer.begin() + BLOCK_SIZE);
    }
}

void SHA1::processBlock(const uint8_t block[BLOCK_SIZE])
{
    uint32_t W[80];
    uint32_t A = digest[0];
    uint32_t B = digest[1];
    uint32_t C = digest[2];
    uint32_t D = digest[3];
    uint32_t E = digest[4];

    for (int i = 0; i < 16; ++i)
    {
        W[i] = byteToUInt32(block + i * 4);
    }
    for (int i = 16; i < 80; ++i)
    {
        W[i] = rotateLeft(W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
    }

    for (int i = 0; i < 80; ++i)
    {
        uint32_t temp = rotateLeft(A, 5) + E + W[i];
        if (i < 20)
        {
            temp += ((B & C) | (~B & D)) + 0x5A827999;
        }
        else if (i < 40)
        {
            temp += (B ^ C ^ D) + 0x6ED9EBA1;
        }
        else if (i < 60)
        {
            temp += ((B & C) | (B & D) | (C & D)) + 0x8F1BBCDC;
        }
        else
        {
            temp += (B ^ C ^ D) + 0xCA62C1D6;
        }
        E = D;
        D = C;
        C = rotateLeft(B, 30);
        B = A;
        A = temp;
    }

    digest[0] += A;
    digest[1] += B;
    digest[2] += C;
    digest[3] += D;
    digest[4] += E;
}

void SHA1::padAndProcess()
{
    buffer.push_back(0x80);
    while ((buffer.size() % BLOCK_SIZE) != 56)
    {
        buffer.push_back(0x00);
    }

    uint64_t length = bitCount;
    for (int i = 0; i < 8; ++i)
    {
        buffer.push_back(static_cast<uint8_t>(length >> (56 - i * 8)));
    }

    processBuffer();
}

void SHA1::reset()
{
    digest[0] = 0x67452301;
    digest[1] = 0xEFCDAB89;
    digest[2] = 0x98BADCFE;
    digest[3] = 0x10325476;
    digest[4] = 0xC3D2E1F0;
    buffer.clear();
    bitCount = 0;
}

uint32_t rotateLeft(uint32_t value, unsigned int bits)
{
    return (value << bits) | (value >> (32 - bits));
}

uint32_t byteToUInt32(const uint8_t bytes[4])
{
    return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

void uint32ToByte(uint32_t value, uint8_t bytes[4])
{
    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = value & 0xFF;
}

#endif
