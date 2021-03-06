import os,sys,random,math

def create_string_entry(input,output):
	output = [str(float(value)) for value in output]
	
	input_string = " ".join([str(float(value)) for value in input])
	output_string = " ".join(output)
	
	return input_string + "|" + output_string + "\n"
	
def generate_binary_set(topology,size):

	# this should check size output <= logbase2(size input)
	# if(topology[-1] != topology[0]+1):
		# print("Invalid topology, exiting")
		# exit(0)

	data_set_as_string = ""

	for i in range(size):
		input_layer = []
		for k in range(topology[0]):
			input_layer.append(random.randrange(0,2))

		_sum = sum(input_layer)
		
		binary = "{0:b}".format(_sum)
		target_output_layer = []
		target_output_layer = list(binary)
		while(len(target_output_layer) <= math.log(len(input_layer),2)):
			target_output_layer.insert(0,0)

		data_set_as_string += create_string_entry(input_layer,target_output_layer)
		
	return data_set_as_string

def generate_random_set(topology,size):

	if(topology[-1] != topology[0]+1):
		print("Invalid topology, exiting")
		exit(0)

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
		
		
def generate_functional_set(topology,size):

	data_set_as_string = ""
	
	if(topology[0] != topology[-1] != 1):
		print("Invalid topology, exiting")
		exit()

	for i in range(size):
	
		input_layer = []
		for k in range(topology[0]):
			input_layer.append(random.randrange(0,2))
		
		target_output_layer = [not k for k in input_layer]

		data_set_as_string += create_string_entry(input_layer,target_output_layer)
		
	return data_set_as_string
	
		

training_set_filename = "training_data.txt"
network_params_filename = "params.txt"

training_set_size = 50000

network_topology = [5,10,12,10,5]
# network_topology = [5,10,10,10,6]

valid = True
for layer in network_topology:
	if(2**network_topology[0] >= layer):
		valid = False 
		break
if not valid:
	print("Warning: Potentially unlearnable topology")

topology_string = ' '.join([str(layer_size) for layer_size in network_topology])

#eta, alpha
params = "0.1 0.1"

data_set_as_string = generate_functional_set(network_topology,training_set_size)
data_set_as_string = data_set_as_string[:-1]

f = open(network_params_filename,"w+")
f.write(topology_string+"\n"+params)
f.close()

f = open(training_set_filename,"w+")
f.write(data_set_as_string)
f.close()

print("Generated",training_set_size,"training samples")