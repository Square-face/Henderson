from contextlib import asynccontextmanager
from fastapi import FastAPI, WebSocket
from bluetooth import Bluetooth
from magic import Command
from manager import ConnectionManager
from bleak.backends.characteristic import BleakGATTCharacteristic
import json

henderson = Bluetooth()
manager = ConnectionManager()

def callback(sender: BleakGATTCharacteristic, data: bytearray):
    print(f"{sender}: {data.hex()}")

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
        data = json.loads(message)

        cmd = Command.get_command(**data)
        await henderson.sendHex(cmd.get_cpids())



        
