#bee_aug = "int bee_index;\nfor(bee_index = 0; bee_index < current_plan_node->bee_number[" + FNAME_heap_compute_data_size + 
#"]; bee_index++)\n{\n\tchar* bee_code = current_plan_node->bee[" + FNAME_heap_compute_data_size
# + "][bee_index];\n\tbee_return_value_holder = ((int (*)()) bee_code)();\n}"
files = []

import sys
infile = open("file:func.txt",'r')
for line in infile:
	last_2_line = ''
	last_line = ''
	cur_line = ''
	fileBuffer = []
	find_include = 0
	fileName = line.split(':')[0]
	funcName = line.split(':')[1][:-1] + '('
	if(fileName not in files):
		files.append(fileName)
		find_include = 1
	else: fileName = '../postgresql_50_bee_caller/' + fileName
	srcfile = open(fileName,'r')
	find_parenthesis = 0
	for src_line in srcfile:
		if(src_line == '\n'): continue
		last_2_line = last_line
		last_line = cur_line
		cur_line = src_line
		if(last_line != ''):
			fileBuffer.append(last_line)
		if(find_include == 1):
			if((last_line.startswith("#include") == True) and (cur_line.startswith("#include")) == False):
				fileBuffer.append('//bee experiment start\n#include"bee_caller.h"\n//bee experiment stop\n')
		if(last_line.startswith(funcName)):
			#print last_2_line + last_line + cur_line
			#find first{
			find_parenthesis = 1
		if(find_parenthesis == 1 and last_line.startswith('{')):
			#print last_line + cur_line
			bee_aug = 'if(current_plan_node != NULL){\n'+ \
			"if(hive_turnON == 1){\nstruct beeList* iterator = current_plan_node->bee[FNAME_" + funcName[:-1] + \
			"]->next;\n" + "while(iterator!=0){\n\tbee_return_value_holder = ((int (*)()) iterator->loc)();\n" + \
			"\titerator = iterator->next;\n\t}\n}\n}\n"

			if(funcName == 'ExecProcNode('):
				bee_aug = 'current_plan_node = node;\n' + bee_aug
			fileBuffer.append('//bee experiment start\n' + bee_aug + '//bee experiment stop\n')
			find_parenthesis = 0
	fileBuffer.append(cur_line)
	srcfile.close()
	outfile = open('../postgresql_50_bee_caller/' + fileName,'w')
	for new_line in fileBuffer:
		outfile.write(new_line)

	outfile.close()
infile.close()
	#if(fileName not in files):
	#	files.append(fileName)
	#for term in fileBuffer:
	#	print term
		
		
