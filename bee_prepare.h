#include "nodes/execnodes.h"
static int beecallers_in_operator[9];
static int operator_beecallers_bitmap[9][2];
static int active_beecallers;
static int wanted_bee_count;

#define cacheSize (8192)
#define IcacheLeadMask 0xfffffffffff80000
#define IcacheTailMask 0x7ffc0

unsigned char addmcode[64];
//"\x55\x48\x89\xe5\xb8\x05\x00\x00\x00\x5d\xc3\x90";
//"\x90\x55\x48\x89\xe5\xb8\x05\x00\x00\x00\x5d\xc3";
void *memory;
int numOfBeeAtSlot[cacheSize];

void  SetBit( int A[ ],  int k );
int TestBit( int A[ ],  int k );


void session_initializer();
void query_initializer();
void bee_memory_allocate();
void beecallers_total(PlanState *planstate);
void bee_placer(PlanState* node);
void bee_placer_helper(PlanState* node,int bee_count_this_operator);
void read_config();
void bee_function_assign(PlanState* node,int bee_count_this_operator);
