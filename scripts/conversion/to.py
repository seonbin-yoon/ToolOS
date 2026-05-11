import sys


def main():
    try:
        num = sys.argv[1]
    except IndexError:
        print("인자 누락")
        sys.exit()

    try:
        dec_num = int(num, 0)
    except ValueError:
        print("잘못된 인자 입력")
        sys.exit()

    kb = dec_num / 1024
    mb = dec_num / 1024 / 1024
    gb = dec_num / 1024 / 1024 / 1024

    output = (
        f"16진수: 0x{dec_num:X}\n"
        f"10진수: {dec_num}\n"
        f"2진수 : {dec_num:b}\n\n"
        f"Byte : {dec_num}\n"
        f"KB   : {kb:.2f}\n"
        f"MB   : {mb:.2f}\n"
        f"GB   : {gb:.2f}"
    )

    print(output)

if __name__ == "__main__":
    main()
