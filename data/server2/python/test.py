import sys
import select

def main():


    print('From cgi: waiting on the servers message', file=sys.stderr)

    print(sys.stdin.readlines())

    # if input_data.strip():
    #     sys.stdout.write(input_data)

    print('From cgi: after writing to the server', file=sys.stderr)

if __name__ == "__main__":
    main()
