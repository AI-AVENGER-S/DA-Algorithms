import random
import string
import sys

def generate_random_string(length, alphabet_size=26):
    """Generate a random string from given alphabet size"""
    alphabet = string.ascii_lowercase[:alphabet_size]
    return ''.join(random.choice(alphabet) for _ in range(length))

def generate_repetitive_string(length, pattern='aaa'):
    """Generate a repetitive pattern string"""
    result = (pattern * (length // len(pattern) + 1))[:length]
    return result

def generate_test_case(test_num, string_length, test_type='random'):
    """Generate a single test case"""
    if test_type == 'random':
        return generate_random_string(string_length)
    elif test_type == 'repetitive':
        return generate_repetitive_string(string_length, 'aaa')
    elif test_type == 'small_alphabet':
        return generate_random_string(string_length, alphabet_size=3)
    elif test_type == 'worst_case':
        return 'a' * (string_length - 1) + 'b'
    else:
        return generate_random_string(string_length)

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 test_generator.py <length> [type]")
        print("Types: random, repetitive, small_alphabet, worst_case")
        sys.exit(1)
    
    length = int(sys.argv[1])
    test_type = sys.argv[2] if len(sys.argv) > 2 else 'random'
    
    test_string = generate_test_case(1, length, test_type)
    print(test_string)

if __name__ == "__main__":
    main()
