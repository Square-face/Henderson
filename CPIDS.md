# The CPIDS communication protocol 

## Requirements

### Required
- Has to be under 20 Bytes as that is the meacimum block size for the BLE module used.
- Has to be able to send Pk, Ik, Dk, speed and state data.

### Ideal
- Be able to bulk send data
- reversable (robot can send its saved data to the host)

## A package 
One CPIDS package consists of 5 parts: Command, Pk, Ik, Dk, Speed.


### Command
The command is one byte and consits of a state code, a fieldmask and a request flag

The state code is a 3 bit number representing the state to change to, alternativly it can be set to 000 and should be ignored

There are 4 available states:
- Standby
- Running
- Calibrating
- Calibrating Manual

  - Pk        >> 4
  - Ik        >> 3
  - Dk        >> 2
  - Speed     >> 1
  - Request   >> 0

### P constant
### I constant
### D constant
### Speed controll

#### IDEA DO NOT IMPLEMENT
last bit of command is checksum

### Example commands:
- get-command --state "standby" -P π-3 -I 0.154255436 -D 0.9876841 -s 124 -r


## States
- Standby
  - -> Calibrating
  - -> Running
- Calibrating
  - -> Standby
- Running
  - -> Standby


### To robot
- Commands
  - run
  - calibrate
  - stop
  - change (PID values and speed)
- PID values
  - Pk
  - Ik
  - Dk
- Speed controll
- Status request


### To host
- Status
  - State
  - PID constants
  - Speed limit
- Logs
  - State
  - Sensor values
  - Line position
  - PID output
  - Motor speeds

