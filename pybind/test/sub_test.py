#!/usr/bin/env python3.12
from pathlib import Path
import sys
import time

sys.path.append((Path.home() / 'projects/up-client-zenoh-cpp/plugin_proposal/build/lib').as_posix())
from pyPluginApi import *

dll = Path.home() / 'projects/up-client-zenoh-cpp/plugin_proposal/build/lib/libapi_implementation.so'
plugin = PluginApi(dll.as_posix())
session = Session(plugin, "start_doc", "ses")

def callback(*args):
    print(f'in callback with {args}')


p1 = Subscriber(session, "upl/*", callback, 4, "sub")

time.sleep(100)

# trace_name = ses
# >>> pub = pyPluginApi.Publisher(s, "upl/p1", "p1")
# >>> quit()