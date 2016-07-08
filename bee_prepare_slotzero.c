#include "postgres.h"
#include "bee_caller.h"
#include "bee_prepare_slotzero.h"
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

unsigned char bee_code[] = "\x8d\x47\x05\xc3\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";


   void  SetBit( int A[ ],  int k )
   {
      A[k/32] |= 1 << (k%32);  // Set the bit at the k-th position in A[i]
   }

   int TestBit( int A[ ],  int k )
   {
      return ( (A[k/32] & (1 << (k%32) )) != 0 ) ;     
   }

void bee_memory_allocate(){
    long pagesize = sysconf(_SC_PAGESIZE);

    if ( posix_memalign(&memory, pagesize, wanted_bee_count*64*cacheSize+sizeof(addmcode)) )
    {
        fprintf(stderr, "Could not allocate memory...\n");
        exit(1);
    }
    memset(memory, 0x90, wanted_bee_count*64*cacheSize+sizeof(addmcode));
    if ( mprotect(memory, wanted_bee_count*64*cacheSize+sizeof(addmcode), PROT_EXEC|PROT_WRITE) )
    {
        perror("mprotect fail");
        exit(1);
    }
}
void bee_placer(PlanState* planstate){
    bee_memory_allocate();
    int bee_count_per_operator = wanted_bee_count / active_beecallers;
    Plan       *plan = planstate->plan;
    bee_placer_helper(planstate,bee_count_per_operator);
    //printf("placed bees %d\n",numOfBeeAtSlot[0]);
}
void bee_placer_helper(PlanState* planstate,int bee_count_per_operator){
    bee_function_assign(planstate,bee_count_per_operator);
    if (planstate->initPlan){
       
        ListCell   *lst;
        foreach(lst, planstate->initPlan)
            {
            SubPlanState *sps = (SubPlanState *) lfirst(lst);
            bee_placer_helper(sps->planstate,bee_count_per_operator);
            }
        }
    /* lefttree */
    if (outerPlanState(planstate))
        bee_placer_helper(outerPlanState(planstate),bee_count_per_operator);
    /* subplan */
    if (planstate->subPlan){
       
        ListCell   *lst;
        foreach(lst, planstate->subPlan)
            {
            SubPlanState *sps = (SubPlanState *) lfirst(lst);
            bee_placer_helper(sps->planstate,bee_count_per_operator);
            }
        }
    /* righttree */
    if (innerPlanState(planstate))
        bee_placer_helper(innerPlanState(planstate),bee_count_per_operator);
    
}
void session_initializer(){
static int session_started = 0;
if(session_started == 0){
    session_started = 1;
    //global variable that should be initialized only once
    //read config file
        memcpy(addmcode,bee_code,sizeof(bee_code));
    read_config();
//printf("array %d\n", sizeof(operator_beecallers_bitmap));
current_plan_node = NULL;
memset(operator_beecallers_bitmap, 0, sizeof(operator_beecallers_bitmap));
    //operator - # of bee callers map
    beecallers_in_operator[OTYPE_NestLoop] =  32;
    beecallers_in_operator[OTYPE_MergeJoin] =  33;
    beecallers_in_operator[OTYPE_HashJoin] =  36;
    beecallers_in_operator[OTYPE_SeqScan] =  41;
    beecallers_in_operator[OTYPE_Material] =  7;
    beecallers_in_operator[OTYPE_Sort] =  26;
    beecallers_in_operator[OTYPE_Agg] =  40;
    beecallers_in_operator[OTYPE_Limit] =  4;
    beecallers_in_operator[OTYPE_ExecHash] =  33;
    //operator - bee caller map
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_ExecProcNode);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_ExecQual);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_ExecProject);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_MemoryContextReset);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_ExecMakeFunctionResultNoSets);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_ExecEvalParamExec);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_pgstat_end_function_usage);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_pgstat_init_function_usage);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_ExecEvalScalarVarFast);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_check_stack_depth);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_int4eq);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_slot_deform_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_numeric_add);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_AllocSetFree);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_slot_getattr);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_make_result);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_mul_var);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_init_var_from_num);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_bcTruelen);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_heap_tuple_untoast_attr);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_palloc);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_add_abs);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_AllocSetAlloc);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_hash_search_with_hash_value);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_heap_form_minimal_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_heap_fill_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_heap_compute_data_size);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_hash_any);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_LWLockAcquire);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_LWLockRelease);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_CheckForSerializableConflictOut);
SetBit(&operator_beecallers_bitmap[OTYPE_NestLoop],FNAME_ReadBuffer_common);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_ExecProcNode);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_MemoryContextReset);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_ExecProject);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_ExecQual);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_ExecMakeFunctionResultNoSets);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_ExecEvalParamExec);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_ExecEvalScalarVarFast);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_ExecStoreTuple);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_pgstat_end_function_usage);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_pgstat_init_function_usage);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_check_stack_depth);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_int4eq);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_slot_deform_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_numeric_add);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_slot_getattr);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_make_result);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_mul_var);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_init_var_from_num);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_bcTruelen);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_palloc);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_heap_fill_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_heap_compute_data_size);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_heap_tuple_untoast_attr);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_AllocSetFree);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_add_abs);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_AllocSetAlloc);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_hash_search_with_hash_value);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_heap_form_minimal_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_hash_any);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_LWLockAcquire);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_LWLockRelease);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_CheckForSerializableConflictOut);
SetBit(&operator_beecallers_bitmap[OTYPE_MergeJoin],FNAME_ReadBuffer_common);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_ExecProcNode);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_ExecHashJoin);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_ExecQual);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_MemoryContextReset);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_ExecProject);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_ExecHashGetHashValue);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_ExecMakeFunctionResultNoSets);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_BufFileWrite);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_ExecEvalParamExec);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_ExecEvalScalarVarFast);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_palloc);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_BufFileRead);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_heap_form_minimal_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_pgstat_end_function_usage);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_pgstat_init_function_usage);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_check_stack_depth);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_int4eq);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_AllocSetFree);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_AllocSetAlloc);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_slot_deform_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_numeric_add);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_slot_getattr);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_heap_fill_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_heap_compute_data_size);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_make_result);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_mul_var);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_init_var_from_num);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_bcTruelen);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_heap_tuple_untoast_attr);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_hash_any);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_add_abs);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_hash_search_with_hash_value);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_LWLockAcquire);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_LWLockRelease);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_CheckForSerializableConflictOut);
SetBit(&operator_beecallers_bitmap[OTYPE_HashJoin],FNAME_ReadBuffer_common);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_ExecScan);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_ExecQual);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_MemoryContextReset);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_ExecProject);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_SeqNext);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_ExecMakeFunctionResultNoSets);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_ExecEvalParamExec);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_ExecStoreTuple);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_heap_getnext);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_ExecProcNode);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_pgstat_end_function_usage);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_pgstat_init_function_usage);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_ExecEvalScalarVarFast);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_check_stack_depth);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_int4eq);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_slot_deform_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_numeric_add);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_heapgettup_pagemode);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_slot_getattr);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_make_result);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_mul_var);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_init_var_from_num);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_bcTruelen);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_heap_tuple_untoast_attr);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_AllocSetFree);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_heapgetpage);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_palloc);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_add_abs);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_HeapTupleSatisfiesMVCC);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_CheckForSerializableConflictOut);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_LWLockRelease);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_AllocSetAlloc);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_hash_search_with_hash_value);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_heap_form_minimal_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_heap_fill_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_heap_compute_data_size);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_XidInMVCCSnapshot);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_ReadBuffer_common);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_LWLockAcquire);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_hash_any);
SetBit(&operator_beecallers_bitmap[OTYPE_SeqScan],FNAME_TransactionIdPrecedes);
SetBit(&operator_beecallers_bitmap[OTYPE_Material],FNAME_ExecProcNode);
SetBit(&operator_beecallers_bitmap[OTYPE_Material],FNAME_palloc);
SetBit(&operator_beecallers_bitmap[OTYPE_Material],FNAME_heap_form_minimal_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_Material],FNAME_AllocSetAlloc);
SetBit(&operator_beecallers_bitmap[OTYPE_Material],FNAME_heap_fill_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_Material],FNAME_heap_compute_data_size);
SetBit(&operator_beecallers_bitmap[OTYPE_Material],FNAME_AllocSetFree);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_ExecProcNode);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_tuplesort_heap_siftup);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_comparetup_heap);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_palloc);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_AllocSetAlloc);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_heap_form_minimal_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_BufFileRead);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_AllocSetFree);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_heap_fill_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_heap_compute_data_size);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_bcTruelen);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_BufFileWrite);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_heap_tuple_untoast_attr);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_int4eq);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_LWLockAcquire);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_LWLockRelease);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_make_result);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_init_var_from_num);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_hash_search_with_hash_value);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_CheckForSerializableConflictOut);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_hash_any);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_add_abs);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_slot_getattr);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_slot_deform_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_ReadBuffer_common);
SetBit(&operator_beecallers_bitmap[OTYPE_Sort],FNAME_MemoryContextReset);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_ExecProcNode);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_ExecQual);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_advance_aggregates);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_ExecProject);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_MemoryContextReset);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_ExecStoreTuple);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_advance_transition_function);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_slot_getattr);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_ExecMakeFunctionResultNoSets);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_ExecEvalParamExec);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_slot_deform_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_int4eq);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_numeric_add);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_tuplesort_heap_siftup);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_AllocSetAlloc);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_pgstat_end_function_usage);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_pgstat_init_function_usage);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_ExecEvalScalarVarFast);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_check_stack_depth);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_hash_search_with_hash_value);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_heap_form_minimal_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_palloc);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_heap_fill_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_heap_compute_data_size);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_bcTruelen);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_make_result);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_init_var_from_num);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_AllocSetFree);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_comparetup_heap);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_mul_var);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_heap_tuple_untoast_attr);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_hash_any);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_add_abs);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_BufFileRead);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_LWLockRelease);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_LWLockAcquire);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_construct_md_array);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_BufFileWrite);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_ReadBuffer_common);
SetBit(&operator_beecallers_bitmap[OTYPE_Agg],FNAME_CheckForSerializableConflictOut);
SetBit(&operator_beecallers_bitmap[OTYPE_Limit],FNAME_ExecProcNode);
SetBit(&operator_beecallers_bitmap[OTYPE_Limit],FNAME_ExecEvalScalarVarFast);
SetBit(&operator_beecallers_bitmap[OTYPE_Limit],FNAME_slot_getattr);
SetBit(&operator_beecallers_bitmap[OTYPE_Limit],FNAME_slot_deform_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_ExecProcNode);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_ExecHashGetHashValue);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_ExecEvalScalarVarFast);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_MemoryContextReset);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_AllocSetAlloc);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_slot_getattr);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_heap_form_minimal_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_ExecProject);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_BufFileWrite);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_slot_deform_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_hash_any);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_bcTruelen);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_palloc);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_heap_fill_tuple);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_heap_compute_data_size);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_ExecMakeFunctionResultNoSets);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_ExecEvalParamExec);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_AllocSetFree);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_heap_tuple_untoast_attr);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_hash_search_with_hash_value);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_pgstat_end_function_usage);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_pgstat_init_function_usage);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_check_stack_depth);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_int4eq);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_numeric_add);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_make_result);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_mul_var);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_init_var_from_num);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_add_abs);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_LWLockRelease);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_LWLockAcquire);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_CheckForSerializableConflictOut);
SetBit(&operator_beecallers_bitmap[OTYPE_ExecHash],FNAME_ReadBuffer_common);

    }
}
void query_initializer(){
    active_beecallers = 0;
}
void beecallers_total(PlanState *planstate){
    Plan       *plan = planstate->plan;
    int operator_type;
    switch (nodeTag(plan))
    {

        case T_NestLoop:
            operator_type = OTYPE_NestLoop;
            break;
        case T_MergeJoin:
            operator_type = OTYPE_MergeJoin;
            break;
        case T_HashJoin:
            operator_type = OTYPE_HashJoin;
            break;
        case T_SeqScan:
            operator_type = OTYPE_SeqScan;
            break;
        case T_Material:
            operator_type = OTYPE_Material;
            break;
        case T_Sort:
            operator_type = OTYPE_Sort;
            break;
        case T_Agg:
            operator_type = OTYPE_Agg;
            break;
        case T_Limit:
            operator_type = OTYPE_Limit;
            break;
        case T_Hash:
            operator_type = OTYPE_ExecHash;
            break;
        default:
            break;
    }
    active_beecallers += beecallers_in_operator[operator_type];
    if (planstate->initPlan){
       
        ListCell   *lst;
        foreach(lst, planstate->initPlan)
            {
            SubPlanState *sps = (SubPlanState *) lfirst(lst);
            beecallers_total(sps->planstate);
            }
        }
    /* lefttree */
    if (outerPlanState(planstate))
        beecallers_total(outerPlanState(planstate));
    /* subplan */
    if (planstate->subPlan){
       
        ListCell   *lst;
        foreach(lst, planstate->subPlan)
            {
            SubPlanState *sps = (SubPlanState *) lfirst(lst);
            beecallers_total(sps->planstate);
            }
        }
    /* righttree */
    if (innerPlanState(planstate))
        beecallers_total(innerPlanState(planstate));

}

void read_config(){
        FILE* config = fopen("/home/yangliu2014/Desktop/wdir/pg_mulhive_slotzero/bee_config_slotzero.txt","r");
        if(config == 0) {printf("error in reading config file\n"); return;}
        fscanf(config,"wanted number of bees: %d\n",&wanted_bee_count);
        fscanf(config,"hive on %d\n",&hive_turnON);
}

void bee_function_assign(PlanState* node,int bee_count_this_operator){
    int i,j;
    uint64_t lead_mask = IcacheLeadMask;
    uint64_t tail_mask = IcacheTailMask;
    uint64_t target_slot,offset,tmp;
    int placementModifier;
    void *fplace;
    Plan       *plan = node->plan;
    int operator_type;
    switch (nodeTag(plan))
    {

        case T_NestLoop:
            operator_type = OTYPE_NestLoop;
            break;
        case T_MergeJoin:
            operator_type = OTYPE_MergeJoin;
            break;
        case T_HashJoin:
            operator_type = OTYPE_HashJoin;
            break;
        case T_SeqScan:
            operator_type = OTYPE_SeqScan;
            break;
        case T_Material:
            operator_type = OTYPE_Material;
            break;
        case T_Sort:
            operator_type = OTYPE_Sort;
            break;
        case T_Agg:
            operator_type = OTYPE_Agg;
            break;
        case T_Limit:
            operator_type = OTYPE_Limit;
            break;
        case T_Hash:
            operator_type = OTYPE_ExecHash;
            break;
        default:
            break;
    }
    for(i = 0; i < 50; i++){
        node->bee_number[i] = 0;
        if(TestBit(&operator_beecallers_bitmap[operator_type],i) == 1){
            node->bee_number[i] = bee_count_this_operator;
            node->bee[i] = malloc(sizeof(struct beeList));
            node->bee[i]->next = 0;
            for(j = 0; j < bee_count_this_operator; j++){
                target_slot = 0;
                //printf("target slot %d, numofbee%d\n",target_slot,numOfBeeCurrentBatch);
                tmp = (uint64_t)memory;
                if((tmp&tail_mask) > target_slot) offset = (tmp&lead_mask) + cacheSize*64;
                else offset = (tmp&lead_mask);
                fplace = (void*)(target_slot|offset);
                placementModifier = numOfBeeAtSlot[0];
                numOfBeeAtSlot[0]++;    
                if(numOfBeeAtSlot[0] >= wanted_bee_count) {
                    //printf("warning of too many bees at slot %d\n",slotSeq[ptr_slot]);
                    continue;}
                //we have room for more bees, so put it
                fplace += placementModifier*cacheSize*64;
//printf("before memcpy\n");
                memcpy(fplace,addmcode, 64);
//printf("after memcpy\n");
                struct beeList* newB = malloc(sizeof(struct beeList));
                newB->loc = fplace;
                newB->next = node->bee[i]->next;
                node->bee[i]->next = newB;
            }
        }
    }


}
