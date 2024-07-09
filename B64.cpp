#include <iostream>
#include <string>
#include <cstdint>
#include <algorithm>

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0, j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while((i++ < 3))
            ret += '=';
    }

    return ret;
}

int64_t decode_base64_to_i64(const std::string& encoded) {
    std::string decoded;
    int val = 0, bits = 0;
    
    for (char c : encoded) {
        if (c == '=') break;
        size_t index = base64_chars.find(c);
        if (index != std::string::npos) {
            val = (val << 6) | index;
            bits += 6;
            if (bits >= 8) {
                bits -= 8;
                decoded += static_cast<char>((val >> bits) & 0xFF);
            }
        }
    }
    
    int64_t result = 0;
    std::copy_n(decoded.begin(), std::min(decoded.size(), sizeof(result)), reinterpret_cast<char*>(&result));
    return result;
}

int main() {
    int64_t original = 1234567890123456789LL;
    std::string encoded = base64_encode(original, 64);
    int64_t decoded = decode_base64_to_i64(encoded);
    
    std::cout << "Original: " << original << std::endl;
    std::cout << "Encoded: " << encoded << std::endl;
    std::cout << "Decoded: " << decoded << std::endl;
    
    return 0;
}