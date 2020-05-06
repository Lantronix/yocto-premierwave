import ctypes
from ctypes import *

_library = ctypes.CDLL('libsecurity.so')

def ltrx_status_get(groups, xcr, messages):
    global _library
    result = _library.ltrx_status_get(ctypes.c_char_p(groups),ctypes.c_char_p(xcr),ctypes.c_char_p(messages))
    return result

def ltrx_cfg_get(groups, xcr, messages):
    global _library
    result = _library.ltrx_cfg_get(ctypes.c_char_p(groups),ctypes.c_char_p(xcr),ctypes.c_char_p(messages))
    return result

def ltrx_cfg_set(groups, xcr, messages):
    global _library
    result = _library.ltrx_cfg_set(ctypes.c_char_p(groups),ctypes.c_char_p(xcr),ctypes.c_char_p(messages))
    return result
