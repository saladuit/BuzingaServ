import sys

print(sys.stdin.readlines())
print("Content-type: text/html")
print("This is from PYTHON", file=sys.stderr)
