import sys

def main():
    # while 1:

    # data = sys.stdin.read()
    # print("Received data from stdin:", data)
    print('Error message 1', file=sys.stderr)

    print("Hello World!", file=sys.stdout)
    sys.stdout.write('Hello world!')

    # sys.stdout.close()

    print('Error message 2', file=sys.stderr)

if __name__ == "__main__":
    main()