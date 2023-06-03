#pragma once

#include "Block.h"


class Worker
{
private:
   Block* data_in = nullptr;
   Block* data_out = nullptr;
   double _bb0, _bb1, _bb2, _ba1, _ba2;
   double _ta1, _ta2, _tb0, _tb1, _tb2;

   bool hasWork = false;
public:
   Worker() = default;
   Worker(Block* in);
   ~Worker();

   /// @brief This function is a part of the INIT process.
   virtual void set_bass(double &bb0, double &bb1, double &bb2, double &ba1, double &ba2);
   /// @brief This function is a part of the INIT process.
   virtual void set_treble(double &tb0, double &tb1, double &tb2, double &ta1, double &ta2);

   /// @brief Set a new block to be processed by this worker (thread)
   /// @return Old/previously saved block
   virtual Block* new_block(Block* in);
   // virtual void write_block(); //TODO: implement this?
   
   virtual void biquad(double b0, double b1, double b2, double a1, double a2);
   virtual void work();
   virtual void swap_blocks();

   bool isDone() const;
};


