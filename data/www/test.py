import sys

def main():
    
    print('Error message 1', file=sys.stderr)

    print("Hello World!", file=sys.stdout)

    print('Error message 2', file=sys.stderr)

if __name__ == "__main__":
    main()