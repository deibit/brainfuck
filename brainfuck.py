import sys

MAX_MEM = 30000

program: list[str] = [
    i for i in open(sys.argv[1]).read() if i in ["<", ">", ".", "[", "]", "+", ",", "-"]
]
jump = {}
s: list[int] = []
for pos, ins in enumerate(program):
    if ins == "[":
        s.append(pos)
    elif ins == "]":
        i = s.pop()
        jump[i] = pos
        jump[pos] = i

assert len(s) == 0, f"Unbalanced [] check these positions: {s}"

memory = [0 for _ in range(MAX_MEM)]
pointer = 0
PC = 0

while PC < len(program):
    assert (
        0 <= PC < len(program) and pointer >= 0
    ), f"Illegal pointer -> {pointer} or PC address -> {PC}"

    i = program[PC]

    if i == "<":
        pointer -= 1
    elif i == ">":
        pointer += 1
    elif i == "+":
        memory[pointer] = memory[pointer] + 1 & 0xFF
    elif i == "-":
        memory[pointer] = memory[pointer] - 1 & 0xFF
    elif i == ".":
        sys.stdout.write(chr(memory[pointer]))
    elif i == "[" and memory[pointer] == 0:
        PC = jump[PC]
    elif i == "]" and memory[pointer] > 0:
        PC = jump[PC]

    PC += 1
