package main

import (
	"fmt"
	"log"
	"os"
	"slices"
)

const maxMemory = 30000

func createJumpTable(program []byte) map[int]int {

	top := 0
	var stack []int
	jump := make(map[int]int)

	for ord, c := range program {
		r := string(c)
		if r == "[" {
			stack = append(stack, ord)
			top++
		} else if r == "]" {
			i := stack[len(stack)-1]
			jump[i] = ord
			jump[ord] = i
			stack = stack[:len(stack)-1]
			top--
		}
	}

	if top != 0 {
		log.Fatal("Error: [] are not balanced in this program")
	}

	return jump
}

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: brainfuck <filename>")
		os.Exit(1)
	}

	program, err := os.ReadFile(os.Args[1])
	if err != nil {
		log.Fatalf("Error: Cannot find or read file: %v", os.Args[1])
	}

	jumpTable := createJumpTable(program)

	memory := make([]int, maxMemory)
	pointer := 0
	pc := 0
	inst_set := []string{"<", ">", "+", "-", "[", "]", "."}

	for pc < len(program) {

		instr := string(program[pc])
		if !slices.Contains(inst_set, instr) {
			pc++
			continue
		}

		// for debug purposes:
		//fmt.Printf("pc: %d ins: %s ptr: %d\n", pc, string(program[pc]), pointer)
		switch instr {
		case "<":
			pointer--
		case ">":
			pointer++
		case "+":
			memory[pointer] = (memory[pointer] + 1) & 0xFF
		case "-":
			memory[pointer] = (memory[pointer] - 1) & 0xFF
		case ".":
			fmt.Printf("%c", memory[pointer])
		case "[":
			if memory[pointer] == 0 {
				pc = jumpTable[pc]
			}
		case "]":
			if memory[pointer] != 0 {
				pc = jumpTable[pc]
			}
		default:
			log.Fatalf("Error: unknown or not implemented instruction %v", instr)
		}

		pc++
	}
}
