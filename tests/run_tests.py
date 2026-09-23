import sys
import os
import os.path as path
import filecmp


def prRed(string):
    print(f"\033[91m{string}\033[00m")


def prGreen(string):
    print(f"\033[92m{string}\033[00m")


def prYellow(string):
    print(f"\033[93m{string}\033[00m")


def prLightPurple(string):
    print(f"\033[94m{string}\033[00m")


def prPurple(string):
    print(f"\033[95m{string}\033[00m")


def prCyan(string):
    print(f"\033[96m{string}\033[00m")


def prLightGray(string):
    print(f"\033[97m{string}\033[00m")


def prBlack(string):
    print(f"\033[98m{string}\033[00m")


help_message = "To run a single test, use 'python3 run_tests.py TEST_NAME\nTo run all tests, run python3 run_tests.py --all"

test_dirs = []


if len(sys.argv) == 1:
    print(help_message)
    quit(0)

if "--all" in sys.argv:
    # we need to run all tests
    test_dirs = os.listdir("simulator")
else:
    test_dirs = [sys.argv[1]]


for test_name in test_dirs:
    test_dir_path = path.join("simulator", test_name)
    if not path.exists(test_dir_path):
        prYellow(f"[ERROR] Test directory for test {test_name} not found")
        continue

    ok_file_path = path.join(test_dir_path, test_name + ".ok")
    if not path.exists(ok_file_path):
        prYellow(f"[ERROR] Correct output file for test {test_name} not found")
        continue

    outfile_path = path.join("output", test_name + ".out")

    if not path.exists("output"):
        os.mkdir("output")

    cmd = "./parmesan"
    cmd += " -q -v 1 "
    cmd += path.join(test_dir_path, test_name)
    cmd += " > "
    cmd += outfile_path
    cmd += " 2>&1 "
    os.system(cmd)

    # Now to compare the output
    (
        prGreen(f"[PASS] {test_name}")
        if filecmp.cmp(ok_file_path, outfile_path, shallow=False)
        else prRed(f"[FAIL] {test_name}")
    )
