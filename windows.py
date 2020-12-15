#!/usr/bin/env python
import os
import matplotlib.pyplot as plt
from ctypes import *

handle = os.add_dll_directory(os.getcwd())

try:
    module = cdll.LoadLibrary("./test.dll")

    module.generate.restype = py_object
    module.generate.argtypes = [ c_double, c_double, c_double, c_double, c_double, c_int, c_double, c_double ]

    fractal = module.generate(-2.5, 5.0, -1.9, 1.0, 0.188, 5, 0.01, 0.01)

    plt.plot([1,2,3,4])
    plt.show()
finally:
    if handle:
        handle.close()