from bleak import BleakClient
from bleak.exc import BleakError

address = "56EFC9A6-7D00-FAC4-9EC4-55E65D5F7A22"
MODEL_NBR_UUID = "00002a25-0000-1000-8000-00805f9b34fb"
COM_UUID = "0000ffe1-0000-1000-8000-00805f9b34fb"

class Bluetooth:
    def __init__(self):
        self.client = BleakClient(address)

    async def connect(self):
        print(f"connecting to ble device {address}...")
        try:
            await self.client.connect()
            print("Connection established")
        except BleakError as _:
            print("Failed to connect to device. Site will still work but no data will be sent")

    async def sendHex(self, msg: str):
        print(f"> {msg[2:]}")
        try:
            await self.client.write_gatt_char(COM_UUID, bytes.fromhex(msg[2:]), False)
        except Exception as _:
            print("Failed to send")

    async def register(self, callback):
        await self.client.start_notify(COM_UUID, callback)

    async def disconnect(self):
        print("Disconnecting ble device")
        await self.client.disconnect()
