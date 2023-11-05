import asyncio
from bleak import BleakClient

address = "56EFC9A6-7D00-FAC4-9EC4-55E65D5F7A22"
MODEL_NBR_UUID = "00002a25-0000-1000-8000-00805f9b34fb"
COM_UUID = "0000ffe1-0000-1000-8000-00805f9b34fb"

async def main(address):
    async with BleakClient(address) as client:
        while True:
            #model_number = await client.read_gatt_char(MODEL_NBR_UUID)
            
            #print("Model Number: {0}".format("".join(map(chr, model_number))))
            string = input("Message to send: ")
            msg = f"{string}\n"

            try:
                print(f"> {msg}", end="")
                await client.write_gatt_char(COM_UUID, msg.encode(), False)
            except Exception as identifier:
                pass


asyncio.run(main(address))
