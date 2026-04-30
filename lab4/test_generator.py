import random
import string

def generate_word(length=8):
    return ''.join(random.choices(string.ascii_letters, k=length))

def generate_test(filename, word_count=1_000_000, pattern_len=3):
    words = [generate_word(random.randint(3, 16)) for _ in range(1000)]
    pattern = [random.choice(words) for _ in range(pattern_len)]
    
    with open(filename, 'w') as f:
        f.write(" ".join(pattern) + "\n")
        
        for i in range(word_count):
            prob = random.random()
            if prob < 0.01:
                f.write(" ".join(pattern) + " ")
            elif prob < 0.05:
                f.write(" ".join(pattern[:pattern_len-1]) + " " + generate_word() + " ")
            else:
                f.write(random.choice(words) + " ")
            
            if i % 10 == 0:
                f.write("\n")

def generate_killer_test():
    with open("test_input.txt", "w") as f:
        pattern = ["a"] * 100 + ["b"]
        f.write(" ".join(pattern) + "\n")
        
        text = ["a"] * 1000000 + ["b"]
        f.write(" ".join(text) + "\n")

# if __name__ == "__main__":
#     generate_killer_test()

if __name__ == "__main__":
    generate_test("test_input.txt", word_count=500_000, pattern_len=4)
    print("Test file 'test_input.txt' generated.")