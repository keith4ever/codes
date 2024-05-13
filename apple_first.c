#include <stdio.h>


// write a function that swaps the odd and even bits in a given integer number
// for example:  11 01 00 10 (0xd2) -> 11 10 00 01 (0xe1)
// Test: 0xA98A139FC2A2650 ->  0x5645236FC1519A0
// can you do it with one line of code?

unsigned long long bitswap(unsigned long long sourcedata){
  return (((sourcedata & 0xAAAAAAAAAAAAAAAA) >> 1) | (((sourcedata & 0x5555555555555555) << 1)));
}
// To execute C, please define "int main()"

int main() {
  unsigned long long ret = bitswap(0xA98A139FC2A2650);
  printf("swapped: 0x%llx\n", ret); 
}


#include <cstdint>
#include <iostream>
#include <queue>
using namespace std;

void avgfilter(uint8_t *input, int boxdim, int width, int height){
  if(boxdim % 2 == 0) return; 
  uint8_t *output = new uint8_t[width * height];
  int xidx, yidx, avg, pixels; 

  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      avg = 0; 
      pixels = boxdim * boxdim;
      for(int k = -(boxdim-1)/2; k < (boxdim-1)/2; k++){
        for(int l = -(boxdim-1)/2; l < (boxdim-1)/2; l++){
          xidx = i + k; yidx = j + l; 
          if(!(xidx < 0 || xidx >= width || yidx < 0 || yidx >= height))
            avg += input[xidx + yidx * height]; 
          else pixels--; 
        }
      }
      output[i + j*height] = avg / pixels; 
    }
  }
  memcpy((char*)input, output, width * height); 
  delete [] output; 
}

Integral image: 
(val(d.x, d.y) - val(a.x, d.y) - val(d.x, a.y) + val(a.x, a.y)) / (boxdim * boxdim)


class Node{
  Node* left;
  Node* right; 
  std::queue<Node*> childlist; 
  int num_children; 
  void addleft(Node* node);
  void addright(Node* node);  
}

int countchild(Node* node){
  if(node == nullptr) return 0; 

  int number = countchild(node->left);
  number += countchild(node->right);
  
  if(number <= 0) number = 1;
  return (number); 
}

// To execute C++, please define "int main()"
int main() {
  Node* root = new Node();

  return 0;
}


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>




// Queue of integers. First-In-First-Out. Fixed capacity.
// Requirement: No per-enqueue/dequeue allocations.
typedef struct Queue {
  // TODO: implement
  int* pqueue; 
  int capacity;
  int size; 
  int begin;
  int end; 
} Queue_t;


Queue_t* Queue_create(int capacity)
{  // Return NULL if initialization was unsuccessful
  Queue_t* queue = (Queue_t*) malloc(sizeof(Queue_t));
  queue->pqueue = (int*) malloc(capacity * sizeof(int));
  queue->capacity = capacity;
  queue->begin = queue->end = queue->size = 0; 
  return queue; 
}


void Queue_destroy(Queue_t* queue)
{
  if(queue == NULL ) return;
  if(queue->pqueue != NULL) 
    free(queue->pqueue); 
  free(queue); 
}


bool Queue_enqueue(Queue_t* queue, int value)
{  // Return true if 'value' is successfully enqueued, else false
  if(queue == NULL || queue->pqueue == NULL || queue->size >= queue->capacity) 
    return false; // TODO: implement

  queue->pqueue[queue->begin] = value;
  queue->begin = (queue->begin+1) % queue->capacity; 
  queue->size++; 
  return true; 
}


bool Queue_dequeue(Queue_t* queue, int* outValue)
{  // Return true if 'outValue' is successfully set, else false
  if(queue == NULL || queue->pqueue == NULL || outValue == NULL || queue->size <= 0)
    return false; // TODO: implement

  *outValue = queue->pqueue[queue->end];
  queue->end = (queue->end+1) % queue->capacity;
  queue->size--; 
  return true;  
}


int main()
{
  const int testCapacity = 10;
  bool success;
  Queue_t* queue = Queue_create(testCapacity);
  if (queue == NULL) {
    printf("Failed to initialize!\n");
    return -1;
  }

  for (int i = 1; i <= testCapacity; i++) {
    success = Queue_enqueue(queue, i);
    if (!success) {
      printf("Failed to enqueue! (%d)\n", i);
      return -1;
    }
  }

  success = Queue_enqueue(queue, 99); // should fail
  if (success) {
    printf("Successfully enqueued when should have failed!\n");
    return -1;
  }

  int outVal = 123;
  for (int i = 1; i <= testCapacity; i++) {
    success = Queue_dequeue(queue, &outVal);
    if (!success) {
      printf("Failed to dequeue! (%d)\n", i);
      return -1;
    }
    if (outVal != i) {
      printf("Dequeued value does not match expectation! (%d != %d)\n", outVal, i);
      return -1;
    }
  }

  success = Queue_dequeue(queue, &outVal);
  if (success) {
    printf("Successfully dequeued when should have failed!\n");
    return -1;
  }

  Queue_destroy(queue);

  printf("All tests passed!!!\n");
  return 0;
}

