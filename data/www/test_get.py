import sys
import select

def main():
    
    print('Error message 1', file=sys.stderr)
    
    sys.stdout.write("Hello world!")

    print('Error message 2', file=sys.stderr)

if __name__ == "__main__":
    main()
