#!/usr/bin/python3

import sys
from pathlib import Path


def get_source_file_lists(source_dirs: list[Path]) -> list[Path]:
    found_source_files: list[Path] = []
    for source_dir in source_dirs:
        if not source_dir.exists():
            raise FileNotFoundError(f"에러: {source_dir}은 존재하지 않는 경로입니다.")

        base_dir = source_dir.resolve()

        for file_path in base_dir.rglob("*"):
            if file_path.is_file() and file_path.suffix in [".c", ".h", "_"]:
                rel_path = file_path.relative_to(base_dir)
                found_source_files.append(rel_path)

    found_source_files.sort()
    return found_source_files

def main(inf_path: Path, source_dirs: list[Path]):
    if not inf_path.exists():
        raise FileNotFoundError(f"에러: {inf_path}를 찾지 못했습니다.")

    found_source_files = get_source_file_lists(source_dirs)

    new: list[str] = []
    with open(inf_path, encoding='utf-8') as inf_file:
        wait = False

        for line in inf_file:
            if line.startswith('[Sources]'):
                new.append(line)

                for found_source_file in found_source_files:
                    new.append(f"  {str(found_source_file)}\n")

                new.append("\n")
                wait = True

            elif wait and line.startswith('['):
                wait = False

            if not wait:
                new.append(line)

    with open(inf_path, 'w', encoding='utf-8') as inf_file:
        inf_file.writelines(new)

    print(f"source.py: {len(found_source_files)}개의 소스 파일이 등록 되었습니다.")

if __name__ == "__main__":
    try:
        main(
            Path(sys.argv[1]),
            [Path(path) for path in sys.argv[2:]]
            )
    except IndexError:
        print("수동으로 실행해선 안됩니다. build.sh를 사용하세요.")
    except (FileNotFoundError) as e:
        print(str(e))
        sys.exit(1)

