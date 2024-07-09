#include <cstdio>
#include "chess.hpp"
#include <string>
#include <iostream>
using namespace std;
using namespace chess;


const std::string base64_chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string encode_i64_to_base64(int64_t value) {
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&value);
    std::string result;
    
    for (int i = 0; i < 8; i += 3) {
        uint32_t n = (uint32_t)bytes[i] << 16;
        if (i + 1 < 8) n |= (uint32_t)bytes[i+1] << 8;
        if (i + 2 < 8) n |= (uint32_t)bytes[i+2];
        
        result += base64_chars[(n >> 18) & 63];
        result += base64_chars[(n >> 12) & 63];
        result += (i + 1 < 8) ? base64_chars[(n >> 6) & 63] : '=';
        result += (i + 2 < 8) ? base64_chars[n & 63] : '=';
    }
    
    return result;
}

int main() {
    FILE* INPUTFP = fopen("../Dataset/ProcessedData.txt", "r");
    FILE* OUTPUTFP = fopen("../Dataset/B64_Data.txt", "w");
    if (INPUTFP == NULL) {
        printf("Error: File not found\n");
        return 1;
    }
    int lines = 0;
    while(true) {
        // Read untill new line
        char line[20000];
        if (fgets(line, 20000, INPUTFP) == NULL) {
            break;
        }
        printf("%s\n", line);
        Board bd;
        vector<string> moves;
        vector<int> Evaluations;
        bool phase = true;
        string saved;
        for(int i=0;line[i]!=0;i++) {
            if (line[i]!=' ') {
                saved.push_back(line[i]);
            } else {
                printf("%s %d\n", saved.c_str(), phase);
                if (saved.size()==0) {
                    continue;
                }
                if (phase) {
                    printf("Move: %s\n", saved.c_str());
                    moves.push_back(saved);
                } else {
                    float evaluation;
                    sscanf(saved.c_str(), "%f", &evaluation);
                    Evaluations.push_back(evaluation*100);
                }
                saved.clear();
                phase = !phase;
            }
        }
        for(int i=0;i<moves.size();i++) {
            bd.makeMove(uci::parseSan(bd, moves[i]));
            cout << bd << endl;
            printf("Evaluation: %d\n", Evaluations[i]);
        }
        break;
    }
}