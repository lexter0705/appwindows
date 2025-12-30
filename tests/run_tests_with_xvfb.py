import subprocess
import sys
import os

def run_x11_tests():
    """Устанавливает X11 переменные окружения и проверяет доступность дисплея"""
    display = ":99"
    os.environ['DISPLAY'] = display

    print(f"Setting DISPLAY={display}")
    xvfb_proc = subprocess.Popen(
        ['Xvfb', display, '-screen', '0', '1024x768x24', '+extension', 'RANDR'],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )
    import time
    time.sleep(3)
    if xvfb_proc.poll() is not None:
        print("ERROR: Xvfb failed to start")
        sys.exit(1)

    print("Xvfb started successfully")
    return xvfb_proc

if __name__ == "__main__":
    xvfb_proc = run_x11_tests()
    test_result = subprocess.run(
        [sys.executable, "-m", "pytest", "tests/", "-v"],
        env=os.environ
    )
    xvfb_proc.terminate()
    xvfb_proc.wait()

    sys.exit(test_result.returncode)