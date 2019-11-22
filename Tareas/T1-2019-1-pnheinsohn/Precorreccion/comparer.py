
def compare(line1, line2):
	line1.sort()
	line2.sort()
	
	# for i in range(len(line1)):
	# 	if line1[i] != line2[i]:
	# 		return False
	'''
	for i in range(len(line1)):
		if line1[i] != line2[i]:
			return False
	'''
	cont = 0
	for i in line1:
		if i in line2:
			cont+=1
	print("se obtuvieron {} elem iguales de {}".format(cont, len(line1)))	
	return True

solution = []
with open("solution.txt", "r") as f:
	for line in f.readlines():
		l = line.strip().split()
		solution.append(list(map(lambda x : float(x), l)))
	
	
output = []
with open("out.txt", "r") as f:
	for line in f.readlines():
		l = line.strip().split()
		output.append(list(map(lambda x : float(x), l)))
	
good = 0	
for i in range(len(output)):
	if compare(solution[i], output[i]):
		good += 1

print("{}%".format(100 * good / len(solution)))