import os

def try_os(command : str):
    res = os.system(command)
    if (res != 0):
        print(f"Command `{command}` failed with code {res}")
        exit(0)

dirpath = os.path.dirname(__file__)
project_path = os.path.dirname(dirpath)
build_dir_path = os.path.join(project_path, "build")

os.chdir(build_dir_path)
try_os("cmake --build .")
os.chdir("Debug")
try_os("final-project.exe")