#include "nodes/execnodes.h"
#define FNAME_heap_compute_data_size 0
#define FNAME_heap_fill_tuple 1
#define FNAME_heap_form_minimal_tuple 2
#define FNAME_slot_deform_tuple 3
#define FNAME_slot_getattr 4
#define FNAME_hash_any 5
#define FNAME_heap_getnext 6
#define FNAME_heapgetpage 7
#define FNAME_heapgettup_pagemode 8
#define FNAME_heap_tuple_untoast_attr 9
#define FNAME_TransactionIdPrecedes 10
#define FNAME_ExecProcNode 11
#define FNAME_ExecEvalParamExec 12
#define FNAME_ExecEvalScalarVarFast 13
#define FNAME_ExecMakeFunctionResultNoSets 14
#define FNAME_ExecProject 15
#define FNAME_ExecQual 16
#define FNAME_ExecScan 17
#define FNAME_ExecStoreTuple 18
#define FNAME_advance_aggregates 19
#define FNAME_advance_transition_function 20
#define FNAME_ExecHashGetHashValue 21
#define FNAME_ExecHashJoin 22
#define FNAME_SeqNext 23
#define FNAME_make_result 24
#define FNAME_pgstat_end_function_usage 25
#define FNAME_pgstat_init_function_usage 26
#define FNAME_ReadBuffer_common 27
#define FNAME_BufFileRead 28
#define FNAME_BufFileWrite 29
#define FNAME_LWLockAcquire 30
#define FNAME_LWLockRelease 31
#define FNAME_CheckForSerializableConflictOut 32
#define FNAME_check_stack_depth 33
#define FNAME_construct_md_array 34
#define FNAME_int4eq 35
#define FNAME_add_abs 36
#define FNAME_init_var_from_num 37
#define FNAME_mul_var 38
#define FNAME_numeric_add 39
#define FNAME_bcTruelen 40
#define FNAME_hash_search_with_hash_value 41
#define FNAME_AllocSetAlloc 42
#define FNAME_AllocSetFree 43
#define FNAME_MemoryContextReset 44
#define FNAME_palloc 45
#define FNAME_comparetup_heap 46
#define FNAME_tuplesort_heap_siftup 47
#define FNAME_HeapTupleSatisfiesMVCC 48
#define FNAME_XidInMVCCSnapshot 49

#define OTYPE_NestLoop 0
#define OTYPE_MergeJoin 1
#define OTYPE_HashJoin 2
#define OTYPE_SeqScan 3
#define OTYPE_Material 4
#define OTYPE_Sort 5
#define OTYPE_Agg 6
#define OTYPE_Limit 7
#define OTYPE_ExecHash 8


PlanState* current_plan_node;
int bee_return_value_holder;
int hive_turnON;
