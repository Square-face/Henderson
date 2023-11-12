import struct

def decode_log(buffer: bytearray):

    # validate log by checking that the last bit of the first byte is 0
    if buffer[0] & 0b00000001 != 0:
        return

    # unpack
    (cycle, delta, inp) = struct.unpack("<H H H", buffer[1:7])
    (P, I, D) = struct.unpack("<f f f", buffer[7:19])
    (O, L, R) = struct.unpack("<B B B", buffer[19:22])

    result = { "type": "log", "cycles_per_log": cycle, "delta_per_log": delta, "line_pos": inp, "p": P, "i": I, "d": D, "output": O, "left": L, "right": R}

    return str(result)
