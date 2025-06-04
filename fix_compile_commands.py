import json
import shutil
from pathlib import Path

# ====== 配置区域 ======
TARGET = "arm-linux-gnueabihf"
SYSROOT = Path("/home/hao/luckfox-pico/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/arm-rockchip830-linux-uclibcgnueabihf/sysroot")
INPUT_FILE = Path("build/compile_commands.json")
BACKUP_FILE = Path("build/compile_commands.original.json")
# =======================

def find_sysroot_includes(sysroot: Path):
    include_flags = []
    for subdir in ["usr/include", "include"]:
        full_path = sysroot / subdir
        if full_path.exists():
            include_flags.append(f"-isystem{full_path}")
    return include_flags

def rewrite_command(command: str, is_cpp: bool, extra_flags: list[str]) -> str:
    if is_cpp:
        new_command = command.replace("arm-rockchip830-linux-uclibcgnueabihf-g++", "clang++")
    else:
        new_command = command.replace("arm-rockchip830-linux-uclibcgnueabihf-gcc", "clang")

    # 避免重复参数
    if "--target" not in new_command:
        new_command += f" --target={TARGET}"
    if "--sysroot" not in new_command:
        new_command += f" --sysroot={SYSROOT}"

    for flag in extra_flags:
        if flag not in new_command:
            new_command += f" {flag}"

    return new_command

def process_compile_commands():
    # 备份原始 compile_commands.json
    if INPUT_FILE.exists():
        shutil.move(INPUT_FILE, BACKUP_FILE)
        print(f"🔄 原始 compile_commands.json 已备份为 {BACKUP_FILE}")
    else:
        print("❌ 找不到 compile_commands.json")
        return

    with open(BACKUP_FILE, "r") as f:
        data = json.load(f)

    include_flags = find_sysroot_includes(SYSROOT)

    for entry in data:
        file = entry["file"]
        command = entry["command"]

        is_cpp = file.endswith((".cpp", ".cxx", ".cc", ".hpp", ".hxx", ".hh"))
        entry["command"] = rewrite_command(command, is_cpp, include_flags)

    with open(INPUT_FILE, "w") as f:
        json.dump(data, f, indent=2)

    print(f"✅ 已生成新的 compile_commands.json，clangd 现在可以正确补全交叉编译项目！")

if __name__ == "__main__":
    process_compile_commands()
