from contextlib import asynccontextmanager
from fastapi import FastAPI, WebSocket
from bluetooth import Bluetooth
from magic import Command
from manager import ConnectionManager
from decoder import decode_log, decode_status
from bleak.backends.characteristic import BleakGATTCharacteristic
import json

henderson = Bluetooth()
manager = ConnectionManager()
buffer = bytearray()

async def callback(_: BleakGATTCharacteristic, data: bytearray):
    print(data)
    global buffer
    
    start_length = len(buffer)

    buffer += data

    if start_length == 0 and len(buffer) == 14:
        print(f"bl({len(buffer)}) < {buffer.hex()}")
        await manager.broadcast(decode_status(buffer))
        buffer.clear()
    
    # clear buffer if it was 0 before and its less than a full block now
    # it means the block just sent will not be followed by anything related as it is not a full block
    if start_length == 0 and len(buffer) != 20:
        print(f"bl({len(buffer)}) < {buffer.hex()}")
        buffer.clear()
        return

    if start_length != 0 and len(buffer) == 38:
        print(f"bl({len(buffer)}) < {buffer.hex()}")
        await manager.broadcast(decode_log(buffer))
        buffer.clear()
        return

    if start_length != 0 and len(buffer) != 38:
        buffer.clear()
        return

@asynccontextmanager
async def lifespan(_: FastAPI):

    # Attempt to connect bluetooth device
    await henderson.connect()
    await henderson.register(callback)

    yield

    # Disconnect connected device
    await henderson.disconnect()


app = FastAPI(lifespan=lifespan)

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await manager.connect(websocket)

    while True:
        message = await websocket.receive_text()
        print(f"ws < {message}")

        try:
            data = json.loads(message)

        except:

            print("failed to convert to json")
            msg = {"type": "error", "msg": f"Failed to parse '{message}' to json"}
            await websocket.send_text(json.dumps(msg))
            continue

        cmd = Command.parse(**data)
        await henderson.sendHex(cmd.get_cpids())
        msg = cmd.get_json()
        msg["type"] = "status"
        await manager.broadcast(json.dumps(msg))



        
