import struct, json

def decode_log(buffer: bytearray):

    # validate log by checking that the last bit of the first byte is 0
    if buffer[0] & 0b00000001 != 0:
        return

    # unpack
    (cycle, delta, inp) = struct.unpack("<H H H", buffer[1:7])
    (P, I, D) = struct.unpack("<f f f", buffer[7:19])
    (O, L, R) = struct.unpack("<b B B", buffer[19:22])
    sensors = struct.unpack("<HHHHHHHH", buffer[22:39])

    result = { "type": "log", "cycles_per_log": cycle, "delta_per_log": delta, "line_pos": inp, "p": f"{P:.9f}", "i": f"{I:.9f}", "d": f"{D:.9f}", "output": O, "left": L, "right": R, "sensors": sensors}

    return json.dumps(result)
