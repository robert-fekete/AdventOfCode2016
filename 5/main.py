from md5 import md5

def solve_first(input_string):

    index = 0
    result = ""

    while len(result) < 8:
        hasher = md5(input_string + str(index))
        hash = hasher.hexdigest()
        if hash[:5] == "00000":
            result += hash[5]
            
        index += 1
        
    return result
    
def solve_second(input_string):

    index = 0
    result = ['-'] * 8

    while any([char == '-' for char in result]):
        hasher = md5(input_string + str(index))
        hash = hasher.hexdigest()
        if hash[:5] == "00000" and hash[5] in ('0', '1', '2', '3', '4', '5', '6', '7'):
            position = int(hash[5])
            if result[position] == '-':
                result[position] = hash[6]
            
        index += 1
        
    return "".join(result)
    
print solve_first("abc")  # 18f47a30
# print solve_first("ugkcyxxp")

# print solve_second("abc")  # 05ace8e3
# print solve_second("ugkcyxxp")