import os

def try_os(command : str):
    res = os.system(command)
    if (res != 0):
        print(f"Command `{command}` failed with code {res}")
        exit(0)

def copy_file(dest_path, src_path):
    with open(dest_path, "wb") as file:
        with open(src_path, "rb") as copy_file:
            file.write(copy_file.read())

dirpath = os.path.dirname(__file__)
project_path = os.path.dirname(dirpath)
build_dir_path = os.path.join(project_path, "build")

os.chdir(project_path)
try_os("wsl rm -rf build")
os.mkdir("build")
os.chdir("build")

libs_dir_path = "C:\\Users\\DungeonMaster\\Desktop\\Main-dir\\Tools\\Libs"
sdl2_path = libs_dir_path + "\\SDL2-2.0.16"
glew_path = libs_dir_path + "\\glew-2.1.0"

try_os(f"cmake .. -G \"Visual Studio 17 2022\" -DGLEW_ROOT=\"{glew_path}\" -DSDL2_ROOT=\"{sdl2_path}\"")
try_os("cmake --build .")

os.chdir("Debug")

copy_file("SDL2.dll", f"{sdl2_path}\\lib\\x64\\SDL2.dll")
copy_file("glew32.dll", f"{glew_path}\\bin\\Release\\x64\\glew32.dll")

try_os("final-project.exe")