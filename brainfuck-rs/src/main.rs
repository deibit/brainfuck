use std::env;
use std::fs;

const MAX_MEM: usize = 30000;

fn main() -> std::io::Result<()> {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        println!("Usage: brainfuck <program>");
        return Ok(());
    }

    let program = fs::read(&args[1])?;

    let mut jumps: Vec<usize> = vec![0; program.len()];
    let mut stack: Vec<usize> = Vec::new();

    for (pos, &token) in program.iter().enumerate() {
        match token {
            b'[' => stack.push(pos),
            b']' => {
                let idx = stack.pop().expect("Unbalanced [] check!");
                jumps[idx] = pos;
                jumps[pos] = idx;
            }
            _ => {}
        }
    }
    assert!(stack.is_empty(), "Unmatched '[' remaining");

    let mut mem = vec![0u8; MAX_MEM];
    let mut dp: usize = 0;
    let mut ip: usize = 0;

    while ip < program.len() {
        match program[ip] {
            b'>' => {
                dp += 1;
                assert!(dp < MAX_MEM, "Data pointer overflow");
            }
            b'<' => {
                dp = dp.checked_sub(1).expect("Data pointer underflow");
            }
            b'+' => mem[dp] = mem[dp].wrapping_add(1),
            b'-' => mem[dp] = mem[dp].wrapping_sub(1),
            b'.' => print!("{}", mem[dp] as char),
            b',' => {}
            b'[' => {
                if mem[dp] == 0 {
                    ip = jumps[ip];
                }
            }
            b']' => {
                if mem[dp] != 0 {
                    ip = jumps[ip];
                }
            }
            _ => {}
        }
        ip += 1;
    }

    Ok(())
}
