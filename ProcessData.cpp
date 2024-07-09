#include <stdio.h>
#include "chess.hpp"
#include <string>
using namespace std;
using namespace chess;

int main() {
    FILE* fp = fopen("../Dataset/Lichess_Trimmed.txt", "r");
    FILE* newfp = fopen("../Dataset/ProcessedData.txt", "w");
    if (fp == NULL) {
        printf("Error: File not found\n");
        return 1;
    }
    int lines = 0;
    while(true) {
        // Read untill new line
        char line[20000];
        if (fgets(line, 20000, fp) == NULL) {
            break;
        }
        int phase = 0;
        /*
        0 : 1.
        1: e4
        2: {
        3: [%eval
        4: -0.03]
        5: [%clk
        6: 0:00:01]
        7: }
        8: 1...
        9: e5
        10: {
        11: [%eval
        12: 0.00]
        13: [%clk
        14: 0:00:01]
        15: }
        */
        string saved;
        bool game_end = false;
        string move;
        for(int i = 0; line[i]!=0; i++) {
            if (line[i] != ' ') {
                if (line[i] == '?' || line[i] == '!') {
                    continue;
                }
                saved.push_back(line[i]);
            }
            else {
                if (saved.size()==0) {
                    continue;
                }
                switch(phase) {
                    case 1:
                        move = saved;
                        break;
                    case 4: {
                        if (saved[0] == '#' || saved[0] == 'm') {
                            game_end = true;
                            break;
                        }
                        float evaluation;
                        saved.pop_back();
                        sscanf(saved.c_str(), "%f", &evaluation);
                        fprintf(newfp, "%s %.2f " ,move.c_str(), evaluation);
                        break;
                    }
                    case 9:
                        move = saved;
                        break;
                    case 12: {
                        if (saved[0] == '#' || saved[0] == 'm') {
                            game_end = true;
                            break;
                        }
                        float evaluation;
                        saved.pop_back();
                        sscanf(saved.c_str(), "%f", &evaluation);
                        fprintf(newfp, "%s %.2f ",move.c_str(), evaluation);
                        break;
                    }
                }
                if (game_end) {
                    break;
                }
                phase ++;
                phase = phase % 16;
                saved.clear();
            }
        }
        fprintf(newfp, "\n");
        if (feof(fp)) {
            break;
        }
        lines++;
        if (lines%1000 == 0) {
            printf("Processed %d lines\n", lines);
        }
    }
}