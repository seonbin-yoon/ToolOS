#!/usr/bin/env python3

import sys
from dataclasses import dataclass
from enum import Enum, auto


class ArgNotFoundError(Exception):
    pass

class NumeralSystem(Enum):
    BIN = auto()
    OCT = auto()
    DEC = auto()
    HEX = auto()
    WORD = auto()

@dataclass(slots=True)
class ConversionInfo:
    value: str
    num_system: NumeralSystem
    int_value: int | None = None

    bit: int | None = None
    byte: int | None = None
    kb: float | None = None
    mb: float | None = None
    gb: float | None = None
    tb: float | None = None

    hex_to_word: str | None = None
    word_to_hex: str | None = None

def main(args: list[str]):
    arg_len = len(args) - 1
    if arg_len < 1:
        raise ArgNotFoundError("변환할 숫자가 1개 이상 입력되어야합니다.")

    for i, value in enumerate(args[1:]):
        print("\033[2J\033[3J\033[H", end="")
        try:
            info = convert_all(value)
            show_info(i + 1, arg_len, info)
        except ValueError as e:
            print(f"[{i + 1}/{arg_len}] {str(e)}")
        message = "스페이스바를 제외한 아무 키나 눌러 다음으로 넘어갑니다.." \
            if i + 1 < arg_len else "스페이스를 제외한 키를 눌러 프로그램을 닫습니다.."

        input(f"\n{message}")

def convert_data_sizes(byte_value: int) -> \
        tuple[int, int, float, float, float, float]:

    byte = byte_value
    bit = byte * 8
    kb = byte / 1024
    mb = byte / 1024 ** 2
    gb = byte / 1024 ** 3
    tb = byte / 1024 ** 4

    return bit, byte, kb, mb, gb, tb

def word_to_hex(word: str) -> str:
    return "".join(format(ord(char), '02X') for char in reversed(word))

def hex_to_word(hex_num: str) -> str:
    clean_hex_num = hex_num.lstrip("-").lower().replace("0x", "")

    if len(clean_hex_num) % 2 != 0:
        clean_hex_num = "0" + clean_hex_num

    try:
        decoded = bytes.fromhex(clean_hex_num).decode('ascii', errors='strict')
    except UnicodeDecodeError:
        return "변환할 수 없는 범위의 값"

    return repr(decoded[::-1])

def convert_all(value: str) -> ConversionInfo:
    value_stripped = value.lstrip("-")

    if value_stripped.startswith("0b"):
        try:
            num = int(value, 2)
            num_system = NumeralSystem.BIN
        except ValueError as e:
            raise ValueError(f"유효하지 않은 2진수입니다: {value}") from e

    elif value_stripped.startswith("0x"):
        try:
            num = int(value, 16)
            num_system = NumeralSystem.HEX
        except ValueError as e:
            raise ValueError(f"유효하지 않은 16진수입니다: {value}") from e

    elif value_stripped.startswith("0o"):
        try:
            num = int(value, 8)
            num_system = NumeralSystem.OCT
        except ValueError as e:
            raise ValueError(f"유효하지 않은 8진수입니다: {value}") from e

    else:
        try:
            num = int(value, 10)
            num_system = NumeralSystem.DEC
        except ValueError:
            num = 0
            num_system = NumeralSystem.WORD

    if num_system == NumeralSystem.WORD:
        info = ConversionInfo(value=value, num_system=num_system)
        info.word_to_hex = word_to_hex(value)
    else:
        info = ConversionInfo(value=value, num_system=num_system, int_value=num)
        info.bit, info.byte, info.kb, info.mb, info.gb, info.tb = \
            convert_data_sizes(num)
        if num_system == NumeralSystem.HEX:
            info.hex_to_word = hex_to_word(value)

    return info

def show_info(num: int, total: int, info: ConversionInfo):
    def convert(num_system: NumeralSystem):
        match num_system:
            case NumeralSystem.BIN:
                return "2진수"
            case NumeralSystem.OCT:
                return "8진수"
            case NumeralSystem.DEC:
                return "10진수"
            case NumeralSystem.HEX:
                return "16진수"
            case NumeralSystem.WORD:
                return "문자"

    print(f"[{num}/{total}] '{info.value}'는 {convert(info.num_system)}입니다.\n")

    if info.int_value is not None:
        print(
            f"  - Bin  : {bin(info.int_value)}\n"
            f"  - Oct  : {oct(info.int_value)}\n"
            f"  - Dec  : {info.int_value}\n"
            f"  - Hex  : {hex(info.int_value)}\n\n"
            f"  - Bit  : {info.bit:,} bits\n"
            f"  - Byte : {info.byte:,} B\n"
            f"  - KB   : {info.kb:,.2f} KB\n"
            f"  - MB   : {info.mb:,.6f} MB\n"
            f"  - GB   : {info.gb:,.8f} GB\n"
        )
    if info.hex_to_word:
        print(f"  - Hex -> Word : {info.hex_to_word}")
    if info.word_to_hex:
        print(f"  - Word -> Hex  : {info.word_to_hex}")

if __name__ == "__main__":
    exit_code = 0
    try:
        main(sys.argv)
    except ArgNotFoundError as e:
        print(str(e))
        exit_code = 1
    except KeyboardInterrupt:
        exit_code = 130

    if not exit_code:
        print("\033[2J\033[3J\033[H", end="")
    sys.exit(exit_code)
