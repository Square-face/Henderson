# ARDUINO CONTROLLER
## Data

Block size: Max 20 Bytes
1 2 2 2 1
c p i d s

### Data structure
- Command
  - first 3 are state
  - Pk
  - Ik
  - Dk
  - Speed
  - Request
- P constant
- I constant
- D constant
- Speed controll

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
  - change (PID values)
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

