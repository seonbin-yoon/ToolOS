import sys
from pathlib import Path


def get_source_file_lists(root_dir: Path, source_dirs: list[Path]) -> list[Path]:
    found_source_file_path: list[Path] = []
    for source_dir in source_dirs:
        if not source_dir.exists():
            raise FileNotFoundError(
                f"source.py: {source_dir} is a path that does not exist."
            )

        for file_path in source_dir.rglob("*"):
            if file_path.is_file() and file_path.suffix in [".c", ".h", "_"]:
                rel_path = file_path.relative_to(root_dir)
                found_source_file_path.append(rel_path)

    found_source_file_path.sort()
    return found_source_file_path

def main(inf_path: Path, root_dir: Path, source_dirs: list[Path]):
    if not inf_path.exists():
        raise FileNotFoundError(f"source.py: File not found {inf_path}")

    found_source_files = get_source_file_lists(root_dir, source_dirs)

    new: list[str] = []
    with open(inf_path, encoding='utf-8') as inf_file:
        wait = False

        for line in inf_file:
            if line.startswith('[Sources]'):
                new.append(line)

                for found_source_file in found_source_files:
                    new.append(f"  {found_source_file!s}\n")

                new.append("\n")
                wait = True

            elif wait and line.startswith('['):
                wait = False

            if not wait:
                new.append(line)

    with open(inf_path, 'w', encoding='utf-8') as inf_file:
        inf_file.writelines(new)

    print(f"source.py: {len(found_source_files)} source files have been registered.")

if __name__ == "__main__":
    try:
        main(
            Path(sys.argv[1]),
            Path(sys.argv[2]),
            [Path(path) for path in sys.argv[3:]]
            )
    except IndexError:
        print("Do not run this manually. Use start.sh.")
    except FileNotFoundError as e:
        print(str(e))
        sys.exit(1)

