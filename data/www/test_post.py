import sys
import select

def main():
    input_data = ""
    newline = False

    rlist = [sys.stdin]
    ready, _, _ = select.select(rlist, [], [])

    if sys.stdin in ready:
        for line in sys.stdin:
            if newline == True:
                break
            for char in line:
                if char == '\n':
                    newline = True
                    break
            input_data += line
        

    print('Error message 1', file=sys.stderr)
    # sys.stdout.write("Hello world!")

    if input_data.strip():
        sys.stdout.write(input_data)
    if input_data.strip():
        sys.stderr.write(input_data)

    print('Error message 2', file=sys.stderr)

if __name__ == "__main__":
    main()
