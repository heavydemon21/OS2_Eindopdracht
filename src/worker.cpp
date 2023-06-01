#include "worker.h"

Worker::Worker(Block* in)
{
   this->data_in = in;
}

Worker::~Worker()
{
   delete data_out;
   data_out = nullptr;
   data_in = nullptr;
}

void Worker::set_bass(double &bb0, double &bb1, double &bb2, double &ba1, double &ba2){
   this->_bb0 = bb0;
   this->_bb1 = bb1;
   this->_bb2 = bb2;
   this->_ba1 = ba1;
   this->_ba2 = ba2;
}
void Worker::set_treble(double &tb0, double &tb1, double &tb2, double &ta1, double &ta2){
   this->_tb0 = tb0;
   this->_tb1 = tb1;
   this->_tb2 = tb2;
   this->_ta1 = ta1;
   this->_ta2 = ta2;
}

Block* Worker::biquad(double b0, double b1, double b2, double a1, double a2){
   int16_t* x = this->data_in->getData();
   int16_t y[BLOCK_SIZE];
   y[0]=x[0];
   for (int n = 0; n < BLOCK_SIZE; n++)
   {
      if (n < 2){ //approximates for unknown data.
         y[n] = (int16_t) b0*x[n] + b1*x[n-n] + b2*x[n-n] + y[n-n]*a1 + a2*y[n-n];
      } else {
         y[n] = b0*x[n] + b1*x[n-1] + b2*x[n-2] + y[n-1]*a1 + a2*y[n-2];
      }
   }
   this->data_out->setData(y);
}

void Worker::swap_blocks(){
   Block* temp = data_out;
   data_out = data_in;
   data_in = temp;
}

void Worker::work(){
   biquad(_bb0, _bb1, _bb2, _ba1, _ba2);
   data_out->setPhase(2);
   swap_blocks();
   biquad(_tb0, _tb1, _tb2, _ta1, _ta2);
   data_out->setPhase(3);
}

Block* Worker::new_block(Block* in){
   Block* temp = data_out;
   data_in = in;
   data_out= in;
   return temp;
}
