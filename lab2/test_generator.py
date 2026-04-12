import sys
import random
import string

def get_random_key():
    str_len = random.randint(1, 256)
    return "".join([random.choice(string.ascii_lowercase) for _ in range(str_len)])

def get_random_value():
    return random.randint(0, 2**64 - 1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <test dir> <count of tests>")
        sys.exit(1)

    test_dir = sys.argv[1]
    count_of_tests = int(sys.argv[2])

    actions = ["+", "-", "?"]

    for enum in range(count_of_tests):
        keys = dict()
        test_file_name = f"{test_dir}/{enum+1:02d}"
        
        with open(f"{test_file_name}.t", 'w') as output_file, \
             open(f"{test_file_name}.a", "w") as answer_file:

            for _ in range(random.randint(10**6 // 1.1, 10**6 // 1.1 + 50_000)):
                action = random.choices(actions, weights=[50, 20, 30], k=1)[0]
                
                if action == "+":
                    key = get_random_key()
                    value = get_random_value()
                    output_file.write(f"+ {key} {value}\n")
                    
                    if key not in keys:
                        answer = "OK"
                        keys[key] = value
                    else:
                        answer = "Exist"
                    answer_file.write(f"{answer}\n")

                elif action == "-":
                    delete_exist_element = random.choice([True, False])
                    if delete_exist_element and len(keys) > 0:
                        key = random.choice(list(keys.keys()))
                    else:
                        key = get_random_key()
                        
                    output_file.write(f"- {key}\n")
                    
                    if key in keys:
                        answer = "OK"
                        del keys[key]
                    else:
                        answer = "NoSuchWord"
                    answer_file.write(f"{answer}\n")