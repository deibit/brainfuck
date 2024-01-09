const fs = require("node:fs");

const ins = ["<", ">", ".", "[", "]", "+", ",", "-"];
const MAX_MEM = 30000;

if (process.argv.length < 3) {
  console.error("Usage: node brainfuck.js <program>");
  process.exit(1);
}

function createJumpTable(program) {
  let jump = new Map();
  let s = new Array();
  let top = 0;
  for (const [idx, inst] of program.entries()) {
    if (inst === "[") {
      s.push(idx);
      top++;
    } else if (inst === "]") {
      const i = s.pop();
      jump[i] = idx;
      jump[idx] = i;
      top--;
    }
  }

  if (top) {
    console.error("Error: check [] balance");
    process.exit(1);
  }

  return jump;
}

try {
  const program = Array.from(fs.readFileSync(process.argv[2], "utf8")).filter(
    (x) => ins.includes(x),
  );

  let pointer = 0;
  let pc = 0;
  let memory = new Array(MAX_MEM).fill(0);

  const jump = createJumpTable(program);

  while (pc < program.length) {
    i = program[pc];

    switch (i) {
      case "<":
        pointer--;
        break;
      case ">":
        pointer++;
        break;
      case "+":
        memory[pointer] = (memory[pointer] + 1) & 0xff;
        break;
      case "-":
        memory[pointer] = (memory[pointer] - 1) & 0xff;
        break;
      case ".":
        process.stdout.write(String.fromCharCode(memory[pointer]));
        break;
      case "[":
        if (memory[pointer] === 0) pc = jump[pc];
        break;
      case "]":
        if (memory[pointer] !== 0) pc = jump[pc];
        break;
      default:
        console.error("Error: Illegal instruction found " + program[pc]);
        process.exit(1);
    }

    pc++;
  }
} catch (err) {
  console.error(err);
}
