from contextlib import asynccontextmanager
from fastapi import FastAPI, WebSocket
from bluetooth import Bluetooth
from magic import Command
from manager import ConnectionManager
from decoder import decode_log
from bleak.backends.characteristic import BleakGATTCharacteristic
import json

henderson = Bluetooth()
manager = ConnectionManager()
buffer = bytearray()

async def callback(_: BleakGATTCharacteristic, data: bytearray):
    global buffer
    print(f"bl({len(data)}) < {data.hex()}")
    
    start_length = len(buffer)

    buffer += data

    if start_length == 0 and len(buffer) != 20:
        buffer.clear()

    if start_length != 0 and len(buffer) == 38:
        await manager.broadcast(decode_log(buffer))
        buffer.clear()

    elif start_length != 0 and len(buffer) != 38:
        buffer.clear()

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
        await manager.broadcast(json.dumps(cmd.get_json()))



        
