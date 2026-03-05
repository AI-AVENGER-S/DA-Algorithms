import sys
import random
import string

def main():
    num_records = int(sys.argv[1]) if len(sys.argv) > 1 else 10000
    out_file = sys.argv[2] if len(sys.argv) > 2 else "input.txt"
    
    alphabet = string.ascii_letters + string.digits + "_-.,:;()[]{}<>@#%&+=?/\\|~^"
    
    with open(out_file, 'w', encoding='utf-8') as f:
        for _ in range(num_records):
            key = random.getrandbits(64)
            val_len = random.randint(1, 100) 
            val = ''.join(random.choices(alphabet, k=val_len))
            f.write(f"{key}\t{val}\n")

if __name__ == "__main__":
    main()