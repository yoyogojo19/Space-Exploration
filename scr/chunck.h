#pragma once
#include <cstdint>
#include <sys/types.h>
#include <vector>
#ifndef CHUNKS_H
#define CHUNKS_H

enum blocks {
    AIR,
    ROCK
};

uint packIstance(uint8_t x, uint8_t y, uint8_t z, uint8_t dir) {
    return (uint)(x & 15u) | (uint)((y & 15u) << 4) | (uint)((z & 15u) << 8) | (uint)((dir & 7u) << 12);
}

uint packPos(uint8_t x, uint8_t y, uint8_t z) {
    return (uint)(x & 15u) | (uint)((y & 15u) << 4) | (uint)((z & 15u) << 8);
}

struct chunck {
    enum blocks data[16][16][16];
    unsigned int pos;
    int size;
    chunck() : size(16), pos(packPos(0,0,0)) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                for (int k = 0; k < size; k++) {
                    data[i][j][k] = blocks(AIR);
                }
            }
        }
    }
    chunck(uint pos) : size(16), pos(pos) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                for (int k = 0; k < size; k++) {
                    data[i][j][k] = blocks(AIR);
                }
            }
        }
    }

    void setBlock(uint8_t x, uint8_t y, uint8_t z, enum blocks id) {
        data[x][y][z] = id;
    }

    enum blocks blockAt(uint8_t x, uint8_t y, uint8_t z) {
        return data[x][y][z];
    }

    bool isAir(int x, int y, int z) {
        if (x > size-1 || x < 0) {
            return true;
        }
        if (y > size-1 || y < 0) {
            return true;
        }
        if (z > size-1 || z < 0) {
            return true;
        }
        return data[x][y][z] == blocks(AIR);
    }

    std::vector<uint> generateTerrin() {
        std::vector<uint> info;
        info.clear();
        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                for (int z = 0; z < size; z++) {
                    int chunkX = x; 
                    int chunkY = y; 
                    int chunkZ = z;
                    if (!isAir(chunkX, chunkY, chunkZ)) {
                        if (isAir(chunkX, chunkY, chunkZ+1)) {
                            info.push_back(packIstance(chunkX, chunkY, chunkZ, 0));
                        }
                        if (isAir(chunkX, chunkY, chunkZ-1)) {
                            info.push_back(packIstance(chunkX, chunkY, chunkZ, 1));
                        }
                        if (isAir(chunkX+1, chunkY, chunkZ)) {
                            info.push_back(packIstance(chunkX, chunkY, chunkZ, 2));
                        }
                        if (isAir(chunkX-1, chunkY, chunkZ)) {
                            info.push_back(packIstance(chunkX, chunkY, chunkZ, 3));
                        }
                        if (isAir(chunkX, chunkY-1, chunkZ)) {
                            info.push_back(packIstance(chunkX, chunkY, chunkZ, 4));
                        }
                        if (isAir(chunkX, chunkY+1, chunkZ)) {
                            info.push_back(packIstance(chunkX, chunkY, chunkZ, 5));
                        }
                    }
                }
            }
        }
        return info;
    }
};


#endif