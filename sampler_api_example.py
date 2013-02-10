import os
import sys
import struct
import time
import unittest

sys.path = ["./py"] + sys.path

import cbox

global Document
Document = cbox.Document

scene = Document.get_scene()
scene.clear()
instrument = scene.add_new_instrument_layer("test_sampler", "sampler").get_instrument()

npfs = instrument.cmd_makeobj("/engine/load_patch_from_string", 0, '.', '', 'new_patch')
instrument.cmd("/engine/set_patch", None, 1, 0)

g1 = npfs.new_group()
g1.set_param("cutoff", "100")
g1.set_param("resonance", "6")
g1.set_param("fil_type", "lpf_4p")
g1.set_param("fileg_decay", "0.2")
g1.set_param("fileg_sustain", "10")
g1.set_param("fileg_depth", "5400")
g1.set_param("fileg_release", "10")
g1.set_param("ampeg_release", "0.1")
g1.set_param("amp_veltrack", "0")
g1.set_param("volume", "-12")

r1 = g1.new_region()
r1.set_param("sample", "*saw")
r1.set_param("transpose", "0")
r1.set_param("tune", "5")

r2 = g1.new_region()
r2.set_param("sample", "*sqr")
r2.set_param("transpose", "12")

print "Ready!"

while True:
    cbox.do_cmd("/on_idle", None, [])
