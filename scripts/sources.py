import os


def update_inf_sources(inf_path: str, source_dirs: list[str]) -> None:
    found_source_files: list[str] = []
    inf_path = os.path.join(os.path.expanduser("~"), inf_path)

    if not os.path.exists(inf_path):
        print(f"에러: {inf_path}를 찾지 못했습니다.")
        return

    for source_dir in source_dirs:
        source_dir = os.path.join(os.path.expanduser("~"), source_dir)
        if not os.path.exists(source_dir):
            print(f"에러: {source_dir}은 존재하지 않는 경로입니다.")
            continue

        full_path = os.path.abspath(source_dir)

        for root, _, files in os.walk(source_dir):
            for file in files:
                if file.startswith('_'):
                    continue

                if file.endswith('.c') or file.endswith('.h'):
                    abs_path = os.path.abspath(os.path.join(root, file))
                    rel_path = os.path.relpath(abs_path, start=full_path)
                    found_source_files.append(rel_path)

    found_source_files.sort()

    with open(inf_path, encoding='utf-8') as inf_file:
        write_lines: list[str] = []
        write: bool = False

        for line in inf_file:
            if line.strip().startswith('[Sources]'):
                write_lines.append(line)

                for found_source_file in found_source_files:
                    write_lines.append(f"  {found_source_file}\n")

                write_lines.append("\n")
                write = True
                continue

            if write and line.strip().startswith('['):
                write = False

            if not write:
                write_lines.append(line)

    with open(inf_path, 'w', encoding='utf-8') as inf_file:
        inf_file.writelines(write_lines)

    print(f"source.py: {len(found_source_files)}개의 소스 파일이 업데이트 되었습니다.")

if __name__ == "__main__":
    update_inf_sources(
        "src/edk2/MdeModulePkg/Application/ToolOS/ToolOS.inf",
        ["ToolOS/boot"]
        )
