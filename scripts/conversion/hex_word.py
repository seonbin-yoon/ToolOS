import sys


def main():
    if len(sys.argv) < 2:
        print("인자 누락")
        sys.exit()

    num = sys.argv[1]

    if num.startswith("0x"):
        num = num[2:]

    result_words = ""

    for i in range(0, len(num), 2):
        hex_byte = num[i:i+2]
        result_words += chr(int(hex_byte, 16))
    word = "".join(reversed(result_words))

    print(f"변환 결과 : {word}")

if __name__ == "__main__":
    main()
