import sys
import select

def main():
    input_data = ""
    newline = False

    print('From cgi: waiting on the servers message', file=sys.stderr)

    rlist = [sys.stdin]
    ready, _, _ = select.select(rlist, [], [])

    print('From cgi: processing the servers message', file=sys.stderr)

    if sys.stdin in ready:
        for line in sys.stdin:
            if newline == True:
                break
            for char in line:
                if char == '\n':
                    newline = True
                    break
            input_data += line

    print('From cgi: before writing to the server', file=sys.stderr)

    if input_data.strip():
        sys.stdout.write(input_data)

    print('From cgi: after writing to the server', file=sys.stderr)

if __name__ == "__main__":
    main()
