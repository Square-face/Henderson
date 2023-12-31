import re
import struct, json

def decode_log(buffer: bytearray):

    # validate log by checking that the last bit of the first byte is 0
    if buffer[0] & 0b00000001 != 0:
        return

    # unpack
    (cycle, delta, inp) = struct.unpack("<H H H", buffer[1:7])
    (P, I, D) = struct.unpack("<f f f", buffer[7:19])
    (O, L, R) = struct.unpack("<h B B", buffer[19:23])
    sensors = struct.unpack("<HHHHHHHH", buffer[23:40])

    result = { "type": "log", "cycles_per_log": cycle, "delta_per_log": delta, "line_pos": inp, "p": f"{P:.9f}", "i": f"{I:.9f}", "d": f"{D:.9f}", "output": O, "left": L, "right": R, "sensors": sensors}

    return json.dumps(result)

def decode_status(buffer: bytearray):

    # unpack
    (state, P, I, D, speed) = struct.unpack("<B f f f B", buffer)

    result = {
            "type": "status", "state": state, "Pk": P, "Ik": I, "Dk": D, "speed": speed
            }

    print(json.dumps(result))

    return json.dumps(result)

