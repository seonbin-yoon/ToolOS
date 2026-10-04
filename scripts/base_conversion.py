#!/usr/bin/env python3

import sys
from dataclasses import dataclass
from enum import Enum


class ConvertError(Exception):
    def __init__(self, numeral_system: str, raw_value: str):
        super().__init__(f"Invalid {numeral_system} number: {raw_value}")

class NumeralSystem(Enum):
    BIN = "Binary"
    OCT = "Octal"
    DEC = "Decimal"
    HEX = "Hexadecimal"
    TXT = "Text"

@dataclass(slots=True)
class ConversionInfo:
    raw_value: str
    system: NumeralSystem
    int_value: int | None = None
    hex_to_word: str | None = None
    word_to_hex: str | None = None

PREFIXES = {
    "0b": (2, NumeralSystem.BIN),
    "0o": (8, NumeralSystem.OCT),
    "0x": (16, NumeralSystem.HEX),
}

def console_clear() -> None:
    print("\033[2J\033[3J\033[H", end="")

def word_to_hex(word: str) -> str:
    return "".join(f"{ord(char):02x}" for char in reversed(word))

def hex_to_word(hex_num: str) -> str:
    clean_hex = hex_num.lower().removeprefix("0x")
    if len(clean_hex) % 2:
        clean_hex = "0" + clean_hex

    try:
        decoded = bytes.fromhex(clean_hex).decode("ascii")
    except ValueError:
        return "Values outside the convertible range"

    return repr(decoded[::-1])

def convert_all(raw_value: str) -> ConversionInfo:
    if not raw_value:
        raise ValueError(f"Empty value: {raw_value}")

    base, numeral_system = PREFIXES.get(raw_value[:2].lower(), (10, NumeralSystem.DEC))

    try:
        num = int(raw_value, base)
    except ValueError as e:
        if base != 10:
            raise ConvertError(numeral_system.value.lower(), raw_value) from e
        return ConversionInfo(
            raw_value,
            NumeralSystem.TXT,
            word_to_hex=word_to_hex(raw_value)
            )

    return ConversionInfo(
        raw_value,
        numeral_system,
        int_value=num,
        hex_to_word=hex_to_word(raw_value) \
            if numeral_system == NumeralSystem.HEX \
            else None,
    )


def show_converted(info: ConversionInfo) -> None:
    print(f"  '{info.raw_value}' is {info.system.value}.\n")

    if info.int_value is not None:
        print(
            f"  - Bin         : {info.int_value:#b}\n"
            f"  - Oct         : {info.int_value:#o}\n"
            f"  - Dec         : {info.int_value}\n"
            f"  - Hex         : {info.int_value:#x}\n\n"
            "   ===== Size is calculated in bytes. =====\n"
            f"  - Bit         : {info.int_value.bit_length() or 1} Bit\n"
            f"  - Byte        : {info.int_value:,} Byte\n"
            f"  - KB          : {info.int_value / 1024:,.2f} KB\n"
            f"  - MB          : {info.int_value / 1024 ** 2:,.6f} MB\n"
            f"  - GB          : {info.int_value / 1024 ** 3:,.8f} GB\n"
        )
    if info.hex_to_word:
        print(f"  - Hex -> Word (le + ascii) : {info.hex_to_word}", end="\n\n")
    if info.word_to_hex:
        print(f"  - Word -> Hex (le) : 0x{info.word_to_hex}", end="\n\n")

def main(values: list[str]) -> None:
    if not values:
        raise SystemExit("You must enter at least one number to convert.")

    for i, value in enumerate(values, start=1):
        console_clear()
        show_converted(convert_all(value.lstrip("-")))

        action = "exit" if i == len(values) else "continue"
        input(f"Press Enter to {action}..")


if __name__ == "__main__":
    try:
        main(sys.argv[1:])
    except KeyboardInterrupt:
        sys.exit(130)
    except Exception as e:
        print(str(e))
        sys.exit(1)
    console_clear()
