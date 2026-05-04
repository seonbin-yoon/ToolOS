import os


def update_inf_sources(inf_path: str, source_dirs: list[str]) -> None:
    valid_files: list[str] = []

    for s_dir in source_dirs:
        if not os.path.exists(s_dir):
            continue

        base_path = os.path.abspath(s_dir)

        for root, _, files in os.walk(s_dir):
            for filename in files:
                # _ 제외, .c와 .h만 포함
                if not filename.startswith('_') and \
                   filename.endswith('.c') or filename.endswith('.h'):

                    # 현재 파일의 절대 경로
                    abs_file_path = os.path.abspath(os.path.join(root, filename))

                    # s_dir(기준 폴더)부터의 상대 경로만 추출
                    # 예: /home/path/boot/utils/Acpi.c -> utils/Acpi.c
                    rel_path = os.path.relpath(abs_file_path, start=base_path)

                    # 윈도우 스타일 경로 방지
                    valid_files.append(rel_path.replace(os.sep, '/'))

    valid_files.sort()

    if not os.path.exists(inf_path):
        print(f"Error: {inf_path} not found.")
        return

    with open(inf_path, encoding='utf-8') as f:
        lines: list[str] = f.readlines()

    new_content: list[str] = []
    skip_mode: bool = False

    for line in lines:
        stripped = line.strip()
        if stripped.startswith('[Sources]'):
            new_content.append(line)
            for src in valid_files:
                new_content.append(f"  {src}\n")
            new_content.append("\n")
            skip_mode = True
            continue
        if skip_mode and stripped.startswith('['):
            skip_mode = False
        if not skip_mode:
            new_content.append(line)

    with open(inf_path, 'w', encoding='utf-8') as f:
        f.writelines(new_content)

    print(f"성공: {len(valid_files)}개의 소스 파일이 업데이트 되었습니다.")

if __name__ == "__main__":
    TARGET_INF: str = "/home/pororo/src/edk2/MdeModulePkg/Application/ToolOS/ToolOS.inf"
    TARGET_DIRS: list[str] = ["/home/pororo/ToolOS/boot"]
    update_inf_sources(TARGET_INF, TARGET_DIRS)
