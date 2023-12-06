import sys
import select

def main():
    input_data = ""  # Initialize the input variable

    # Create a list containing the stdin file descriptor
    rlist = [sys.stdin]

    # Wait until there is data available on the stdin
    ready, _, _ = select.select(rlist, [], [])

    if sys.stdin in ready:
        # Read from stdin
        for line in sys.stdin:
            input_data += line

    print('Error message 1', file=sys.stderr)

    sys.stdout.write("Hello world!")

    if input_data.strip():  # Check if input is not an empty string
        sys.stdout.write(input_data)

    print('Error message 2', file=sys.stderr)

if __name__ == "__main__":
    main()
