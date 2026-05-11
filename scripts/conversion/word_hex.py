import sys


def main():
    if len(sys.argv) < 2:
        print("인자 누락")
        sys.exit()

    words = sys.argv[1]

    word_ascii = ""

    for word in reversed(words):
        word_ascii += format(ord(word), '02X')

    print(f"문자 : 0x{word_ascii}")

if __name__ == "__main__":
    main()
