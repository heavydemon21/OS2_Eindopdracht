#pragma once
#include <cstdint>

#define BLOCK_SIZE 1024

class Block
{
private:
    int16_t data[BLOCK_SIZE];
    int phase; // 0 = created, 1 = filled,  2 = bassed, 3 = bassed+trebled, 4 = written
    int index;

public:
    Block() = default;
    Block(int newIndex)
    {
        index = newIndex;
        phase = 0;
    }

    void setData(int16_t* newData)
    {
        for (int i = 0; i < BLOCK_SIZE; i++)
        {
            data[i] = newData[i];
        }
    }

    int16_t* getData(){
        return data;
    }

    void setPhase(int newPhase)
    {
        phase = newPhase;
    }

    int getPhase()
    {
        return phase;
    }

    int getIndex()
    {
        return index;
    }
};