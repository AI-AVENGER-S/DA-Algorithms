import sys

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 checker.py <file_to_check>")
        sys.exit(1)

    path = sys.argv[1]
    prev_key = -1
    line_no = 0
    errors = 0

    with open(path, "r", encoding="utf-8") as f:
        for raw_line in f:
            line_no += 1
            line = raw_line.rstrip('\r\n')
            
            if not line: continue
            
            parts = line.split("\t")
            if len(parts) < 2:
                print(f"Error at line {line_no}: No tab separator. (Line content: '{line}')")
                errors += 1
                continue
            
            try:
                key = int(parts[0])
                if key < prev_key:
                    print(f"Sort Error at line {line_no}: Key {key} < Previous {prev_key}")
                    errors += 1
                prev_key = key
            except ValueError:
                print(f"Error at line {line_no}: Key is not an integer.")
                errors += 1

    if errors == 0:
        print(f"Check OK: {line_no} lines scanned, file is sorted.")
        sys.exit(0)
    else:
        print(f"Check Failed: {errors} errors found.")
        sys.exit(1)


if __name__ == "__main__":
    main()