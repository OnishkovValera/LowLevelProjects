# -*- coding: utf-8 -*-
import subprocess

input_list = ["third word", "second word", "first word", "integer", "lol", "", "lol" * 100, "short"]
output_list = ["third word explanation", "second word explanation", "first word explanation", "size is 32 bits", "", "", "", "size is 16 bits"]
error_list = ["","","","", "Value is not found", "Value is not found", "Input string is too long", ""]

for i in range(len(input_list)):
	subproc = subprocess.Popen(["./program"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	stdout, stderr = subproc.communicate(input=input_list[i].encode())
	out_message = stdout.decode().strip()
	error_message = stderr.decode().strip()
	
	if error_message == error_list[i] and out_message == output_list[i]:
		print("Test {} is successful".format(i + 1))
		if out_message == "":
			print(error_message)
		else:
			print("OUTPUT: {}".format(out_message))
	else:
		print("Test {} failed".format(i + 1))
		print("OUTPUT: {}".format(out_message))
		print(error_message)

