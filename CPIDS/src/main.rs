use clap::{Arg, App};
use std::process;

/// The different states defined by the CPIDS protocol
enum CPIDSStates {
    /// No state change, continue with last state
    Nothing,

    /// Change state to standby
    Standby,

    /// Change state to running
    Running,

    /// Change state to calibrating
    Calibrating,

    /// Change state to manual calibration
    CalibratingManual,
}


/// Structure of a CPIDS command
///
/// The CPIDS protocol uses 14 bytes for a single transmission. These bytes are split up in the following way
/// - 1 Byte - Command
///     Three bites of state code as defined in [CPIDSStates]
///     
///     The following 4 bits are a bitmask for the 4 parameters.
///     i.e if the command is xxx0010x only the derivative will be looked at, all else are ignored
///     This is used so that one can target a specific field to change without knowing anything
///     about what the others should be
///
///     The last byte is a request flag. If it is set it means the reciving device should send all
///     its config data 
///
/// - 4 Bytes - Proportional
///     Floating point number between 0 and 1.
///
///     This float is used to set the proportional constant for the PID controler on the device, if the first
///     bit in the bitmask from the command byte isn't set, this value should be ignored by the
///     device
///
/// - 4 Bytes - Integral
///     Floating point number between 0 and 1.
///
///     This float is used to set the integral constant for the PID controler on the device, if the second 
///     bit in the bitmask from the command byte isn't set, this value should be ignored by the
///     device
///
/// - 4 Bytes - Derivative
///     Floating point number between 0 and 1.
///
///     This float is used to set the derivative constant for the PID controler on the device, if the third 
///     bit in the bitmask from the command byte isn't set, this value should be ignored by the
///     device
///
/// - 1 Byte - Speed
///     Unsigned 8 bit integer. 0 to 255
///
///     This number is used to set the max traversal speed of the target device
#[derive(Debug)]
struct CommandStructure{
    command: u8,
    proportional: f32,
    integral: f32,
    derivative: f32,
    speed: u8
}




/// Get state enum from input string
///
/// Converts user input to a valid [StateCommand] enum value. If not match is found the
/// function will return Err()
///
/// # Arguments
/// * `state_string` - User input
///
/// # Returns
/// 
/// Enum representing the users input.
///
/// # example
///
/// ```rust
/// let state = get_state("nothing").unwrap();
/// println!("{}", state as u8); //prints "0"
/// ```
fn get_state(state_string: &str) -> Result<CPIDSStates, &str> {
    
    match state_string {
        "standby" => Ok(CPIDSStates::Standby),

        "running" => Ok(CPIDSStates::Running),

        "calibrating" => Ok(CPIDSStates::Calibrating),

        "calibrating_manual" => Ok(CPIDSStates::CalibratingManual),
        
        "nothing" => Ok(CPIDSStates::Nothing),
        
        _ => Err("Input doesn't match any valid state('standby', 'running', 'calibrating', 'calibrating-manual', 'nothing')"),
    }
}



fn update_state(input: &str, cmd: &mut CommandStructure) {

    match get_state(input) {
        Ok(state) => cmd.command = (state as u8) << 5,

        Err(msg) => {eprint!("{}", msg); process::exit(1)},
    }
}




/// Combines boolean values into an 8-bit command mask.
///
/// The function takes five boolean parameters `p`, `i`, `d`, `s`, and `r`,
/// representing different command options. These boolean values are combined
/// into a single 8-bit integer using bitwise OR and left shift operations.
///
/// # Arguments
///
/// * `p` - Proportional term flag.
/// * `i` - Integral term flag.
/// * `d` - Derivative term flag.
/// * `s` - Speed term flag.
/// * `r` - Request Status flag.
///
/// # Returns
///
/// An 8-bit integer representing the combined command mask.
///
/// # Examples
///
/// ```
/// let p = true;
/// let i = false;
/// let d = true;
/// let s = false;
/// let r = true;
///
/// let result = get_command_mask(p, i, d, s, r);
///
/// println!("Result: {:08b}", result);
/// ```
fn get_command_mask(
    p: bool,
    i: bool,
    d: bool,
    s: bool,
    r: bool,
) -> u8 {
    return ((p as u8) << 4) | ((i as u8) << 3) | ((d as u8) << 2) | ((s as u8) << 1) | (r as u8);
}



fn main() {
    // Define CLI params
    let matches = App::new("Generate message blocks for Henderson")
        .arg(Arg::with_name("state")
             .help("Target state")
             .long("state")
             .takes_value(true)
             )
        .arg(Arg::with_name("proportional")
             .help("Set a new proportional constant. Takes a float between 0 and 1")
             .short('p')
             .long("proportional")
             .takes_value(true)
             )
        .arg(Arg::with_name("integral")
             .help("Set a new integral constant. Takes a float between 0 and 1")
             .short('i')
             .long("integral")
             .takes_value(true)
             )
        .arg(Arg::with_name("derivative")
             .help("Set a new derivative constant. Takes a float between 0 and 1")
             .short('d')
             .long("derivative")
             .takes_value(true)
             )
        .arg(Arg::with_name("speed")
             .help("Set a new max speed. Takes a value from 0 to 255")
             .short('s')
             .long("speed")
             .takes_value(true)
             )
        .arg(Arg::with_name("request")
             .help("Request status update")
             .short('r')
             .long("request")
             .takes_value(false)
             )
        .get_matches();

    // base Command
    let mut cmd = CommandStructure{
        command: 0x0,
        proportional: 0.0,
        integral: 0.0,
        derivative: 0.0,
        speed: 0x0
    };

    let state = matches.value_of("state").unwrap_or("nothing");
    update_state(state, &mut cmd);

    cmd.proportional    = matches.value_of("proportional").unwrap_or("0.0").parse::<f32>().expect("PID values have to be floats");
    cmd.integral        = matches.value_of("integral").unwrap_or("0.0").parse::<f32>().expect("PID values have to be floats");
    cmd.derivative      = matches.value_of("derivative").unwrap_or("0.0").parse::<f32>().expect("PID values have to be floats");

    cmd.speed           = matches.value_of("speed").unwrap_or("0").parse::<u8>().expect("Speed must be integer from 0 to 255");


    cmd.command = cmd.command | get_command_mask(
        matches.is_present("proportional"),
        matches.is_present("integral"),
        matches.is_present("derivative"),
        matches.is_present("speed"),
        matches.is_present("request"),
        );

    println!("c: {:08b} | {}", cmd.command, cmd.command);
    println!("p: {:032b} | {}", cmd.proportional.to_bits(), cmd.proportional);
    println!("i: {:032b} | {}", cmd.integral.to_bits(), cmd.integral);
    println!("d: {:032b} | {}", cmd.derivative.to_bits(), cmd.derivative);
    println!("s: {:08b} | {}", cmd.speed, cmd.speed);

    println!("{:#04x}{:08x}{:08x}{:08x}{:02x}", cmd.command, cmd.proportional.to_bits(), cmd.integral.to_bits(), cmd.derivative.to_bits(), cmd.speed);
}
