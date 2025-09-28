#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 100
#define MAX_MEN 30000

typedef struct Program {
  char *data;
  size_t len;
} Program;

Program read_program(const char *filename) {
  FILE *f;
  size_t size;
  char *buf;

  f = fopen(filename, "rb");
  if (!f) {
    printf("File not found\n");
    exit(1);
  }

  fseek(f, 0, SEEK_END);
  size = ftell(f);
  fseek(f, 0, SEEK_SET);

  buf = malloc(sizeof(char) * size + 1);
  if (!(buf)) {
    printf("Error allocating a buffer for program");
    fclose(f);
    exit(1);
  }

  fread(buf, 1, size, f);
  buf[size] = '\0';
  fclose(f);
  return (Program){.data = buf, .len = size};
}

int *create_jump_table(const Program program) {
  int *table = calloc(program.len, sizeof(int));

  int stack[STACK_SIZE] = {0};
  int top = 0;

  for (int p = 0; p < program.len; p++) {
    if (*(program.data + p) == '[') {
      assert(top < STACK_SIZE);
      stack[top++] = p;
    } else if (*(program.data + p) == ']') {
      *(table + p) = stack[--top];
      *(table + stack[top]) = p;
    }
  }

  assert(top == 0);

  return table;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    puts("Usage: brainfuck <program>");
    exit(1);
  }
  Program program = read_program(argv[1]);

  int pointer = 0;
  int pc = 0;
  unsigned char memory[MAX_MEN] = {0};

  int *table = create_jump_table(program);

  while (pc < program.len) {
    unsigned char *p_memory = memory + pointer;
    char inst = *(program.data + pc);

    if (inst == '<') {
      pointer--;
    } else if (inst == '>') {
      pointer++;
    } else if (inst == '+') {
      *p_memory = (*p_memory + 1) & 0xff;
    } else if (inst == '-') {
      *p_memory = (*p_memory - 1) & 0xff;
    } else if (inst == '[' && *p_memory == 0) {
      pc = *(table + pc);
    } else if (inst == ']' && *p_memory != 0) {
      pc = *(table + pc);
    } else if (inst == '.') {
      putchar(*p_memory);
    } else if (inst == ',') {
      *p_memory = getchar();
    }
    pc++;
  }
  free(program.data);
  free(table);
  return 0;
}
