import sys
import select

def main():
    
    print('From cgi: before writing \"Hello world!\"', file=sys.stderr)
    
    sys.stdout.write("Hello world!")

    print('From cgi: after writing \"Hello world!\"', file=sys.stderr)

if __name__ == "__main__":
    main()
