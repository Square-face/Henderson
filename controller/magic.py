import subprocess
from enum import Enum


class State(Enum):
    NOTHING = 0
    STANDBY = 1
    RUNNING = 2
    CALIBRATING = 3
    CALIBRATING_MANUAL = 4


class Command:
    def __init__(self):
        self.target_state = State.NOTHING
        
        self.proportional = None
        self.integral = None
        self.derivative = None

        self.speed = None

        self.request_status = False

    def get_cpids(self):
        cmd = ["../command-generator/target/release/command-generator"]


        cmd.append("--state")
        cmd.append(self.target_state.name.lower())


        if self.proportional is not None:
            cmd.append("-p")
            cmd.append(str(self.proportional))


        if self.integral is not None:
            cmd.append("-i")
            cmd.append(str(self.integral))


        if self.derivative is not None:
            cmd.append("-d")
            cmd.append(str(self.derivative))


        if self.speed is not None:
            cmd.append("-s")
            cmd.append(str(self.speed))


        if self.request_status:
            cmd.append("-r")


        cpids = subprocess.run(cmd, capture_output = True, encoding = "utf-8", text = True)
        print(cpids)
        return cpids.stdout.split("\n")[-2]

    def get_json(self):
        cmd = {}

        if self.target_state is not None:
            cmd["state"] = self.target_state.value

        if self.proportional is not None:
            cmd["Pk"] = self.proportional

        if self.integral is not None:
            cmd["Ik"] = self.integral

        if self.derivative is not None:
            cmd["Dk"] = self.derivative

        if self.speed is not None:
            cmd["speed"] = self.speed

        return cmd

    @staticmethod
    def parse(**attr):
        
        cmd = Command()

        if "state" in attr.keys():
            cmd.target_state = State(attr["state"])

        if "Pk" in attr.keys():
            cmd.proportional = attr["Pk"]

        if "Ik" in attr.keys():
            cmd.integral = attr["Ik"]

        if "Dk" in attr.keys():
            cmd.derivative = attr["Dk"]

        if "speed" in attr.keys():
            cmd.speed = attr["speed"]

        if "request_status" in attr.keys():
            cmd.request_status = attr["request_status"]

        return cmd
        
