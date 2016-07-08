bee_code_size = 6
bee_code = '"\\x8d\\x47\\x05\\xc3\\x90'
bee_code_padding = '\\x90'
bee_code_tail = '";'
count = len(bee_code.split('x')) - 1
while(count < bee_code_size):
	bee_code = bee_code + bee_code_padding
	count = count + 1
bee_code = bee_code + bee_code_tail
print bee_code
