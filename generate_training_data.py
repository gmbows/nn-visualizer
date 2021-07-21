import os,sys,random

def create_string_entry(input,output):
	output = [str(float(value)) for value in output]
	
	input_string = " ".join([str(float(value)) for value in input])
	output_string = " ".join(output)
	
	return input_string + "|" + output_string + "\n"

def generate_random_set(topology,size):

	data_set_as_string = ""

	for i in range(size):
		input_layer = []
		for k in range(topology[0]):
			input_layer.append(random.randrange(0,2))

		_sum = sum(input_layer)
		
		target_output_layer = []
		for k in range(topology[-1]):
			target_output_layer.append(0)
			
		target_output_layer[_sum] = 1

		data_set_as_string += create_string_entry(input_layer,target_output_layer)
		
	return data_set_as_string
	
def generate_uniform_set(topology,size):
	
	data_set_as_string = ""
	input_layer = []
	
	for w in range(topology[0]):
		input_layer.append(0)

	for i in range(size):
	
		for k in range(len(input_layer)):
			if input_layer[k] == 1:
				for j in range(k,len(input_layer)):
					if(input_layer[j] == 1):
						input_layer[j] = 0
					elif(input_layer[j] == 0):
						input_layer[j] = 1
						break
				break
			else:
				input_layer[k] = 1
				break

		_sum = sum(input_layer)
		
		target_output_layer = []
		for k in range(topology[-1]):
			target_output_layer.append(0)
			
		target_output_layer[_sum] = 1

		data_set_as_string += create_string_entry(input_layer,target_output_layer)
		
	return data_set_as_string
	
def generate_simple_set(topology,size):
	
	data_set_as_string = ""

	for i in range(size):
	
		input_layer = []
		for k in range(topology[0]):
			input_layer.append(random.randrange(0,2))

		res = sum(input_layer) == 2
		
		target_output_layer = []
		for k in range(topology[-1]):
			target_output_layer.append(0)
			
		target_output_layer[0] = res

		data_set_as_string += create_string_entry(input_layer,target_output_layer)
		
	return data_set_as_string
		
		
training_set_filename = "training_data.txt"

training_set_size = 50000

network_topology = [4,4,4,1]

topology_string = ' '.join([str(layer_size) for layer_size in network_topology])+"\n"
data_set_as_string = generate_simple_set(network_topology,training_set_size)
data_set_as_string = topology_string+data_set_as_string[:-1]

f = open(training_set_filename,"w+")
f.write(data_set_as_string)
f.close()

print("Generated",training_set_size,"training samples")