import subprocess


class Command:
    def __init__(self):
        self.target_state = None
        
        self.proportional = None
        self.integral = None
        self.derivative = None

        self.speed = None

        self.request_status = False

    def get_cpids(self):
        cmd = ["../command-generator/target/release/command-generator"]


        if self.target_state is not None:
            cmd.append("--state")
            cmd.append(self.target_state)


        if self.proportional is not None:
            cmd.append("-p")
            cmd.append(self.proportional)


        if self.integral is not None:
            cmd.append("-i")
            cmd.append(self.integral)


        if self.derivative is not None:
            cmd.append("-d")
            cmd.append(self.derivative)


        if self.speed is not None:
            cmd.append("-s")
            cmd.append(self.speed)


        if self.request_status:
            cmd.append("-r")


        cpids = subprocess.run(cmd, capture_output = True, encoding = "utf-8", text = True)
        print(cpids)
        return cpids.stdout.split("\n")[-2]

    @staticmethod
    def get_command(**attr):
        
        cmd = Command()

        if "state" in attr.keys():
            cmd.target_state = attr["state"]

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
        
