<img src="https://raw.githubusercontent.com/lexter0705/appwindows/refs/heads/master/appwindows.svg" alt="" width="40%" >

[![License - BSD 3-Clause](https://img.shields.io/pypi/l/appwindows.svg)](https://github.com/lexter0705/appwindows/blob/master/LICENSE.md) [![unit_tests](https://github.com/lexter0705/appwindows/actions/workflows/unit_tests.yml/badge.svg)](https://github.com/lexter0705/appwindows/actions/workflows/unit_tests.yml)
<br>
[![PyPI Downloads](https://static.pepy.tech/personalized-badge/appwindows?period=total&units=INTERNATIONAL_SYSTEM&left_color=GRAY&right_color=GREEN&left_text=downloads)](https://pepy.tech/projects/appwindows) 
[![Documentation](https://img.shields.io/badge/docs-gitbook-green)](https://apparser.gitbook.io/appwindows)
<br>
[![PyPi](https://img.shields.io/badge/PyPi-link-green)](https://pypi.org/project/appwindows/)
[![Github](https://img.shields.io/badge/github-repo-green)](https://github.com/lexter0705/appwindows)
[![Issues](https://img.shields.io/badge/github-issues-green)](https://github.com/lexter0705/appwindows/issues)



# Appwindows
The appwindows library provides a unified way to find, inspect, and control application windows across Windows, macOS, and Linux.

# Supported platforms:
1) Windows 10 and later
2) MacOS 12.2 and later
3) Linux with XServer (Wayland works, but with very significant limitations)

# Install
```bash
pip install appwindows
```

# Examples

1) Create window screenshot
```python
from appwindows import get_finder
from appwindows.geometry import Size

finder = get_finder()

window = finder.get_window_by_title("Some title")

window.get_screenshot() # numpy.ndarray whith image
```

2) Move window
```python
from appwindows import get_finder
from appwindows.geometry import Point

finder = get_finder()

window = finder.get_window_by_title("Some title")

window.move(Point(x=200, y=200))
```

3) Get window coordinates and size
```python
from appwindows import get_finder

finder = get_finder()

window = finder.get_window_by_title("Some title")

points = window.get_points()
size = window.get_size()

print(points.left_top) # appwindows.geometry.Point(x=200, y=200)
print(size)            # appwindows.geometry.Size(width=200, height=200)
```
# Why "appwindows"?
| Comparison Point | PyGetWindow | PyWinCtl | appwindows |
| :--- | :---: | :---: | :---: |
| **Cross-Platform** (macOS, Linux, Windows) | ❌ | ✅ | ✅ |
| **Can Take Screenshots** | ❌ | ❌ | ✅ |
| **Actively Maintained** | ❌ | ❌ | ✅ |
| **Has CI/CD Testing** | ❌ | ❌ | ✅ |

# Docs
All documentation <a href="https://apparser.gitbook.io/appwindows">here</a> <br>
Link to <a href="https://pypi.org/project/appwindows/">PyPi</a>
# For Developers
1) If something doesn't work - open issue.
2) If you want something fixed - open issue.
3) If you can help with the library - email.

apparser.development@gmail.com

Any help in development is welcome)!


