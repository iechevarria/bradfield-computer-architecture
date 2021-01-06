#include <stdio.h>

// instructions
#define LOAD_WORD 0x01
#define STORE_WORD 0x02
#define ADD 0x03
#define SUB 0x04
#define HALT 0xff
#define BLANK 0x00

// registers
#define PROGRAM_COUNTER 0x00
#define REG1 0x01
#define REG2 0x02


int read_word(int memory[], int index) {
  return memory[index] + (256 * memory[index + 1]);
}


void write_word(int memory[], int index, int value) {
  memory[index] = value % 256;
  memory[index + 1] = value / 256;
} 


void execute(int memory[]) {
  int registers[3];

  while (registers[PROGRAM_COUNTER] < 14) {

    int pc = registers[PROGRAM_COUNTER];
    int instruction = memory[pc];

    if (instruction == HALT) {
      break;
    } else if (instruction == LOAD_WORD) {
      registers[memory[pc + 1]] = read_word(memory, memory[pc + 2]);
    } else if (instruction == STORE_WORD) {
      write_word(memory, memory[pc + 2], registers[memory[pc + 1]]);
    } else if (instruction == ADD) {
      registers[memory[pc + 1]] += registers[memory[pc + 2]];
    } else if (instruction == SUB) {
      registers[memory[pc + 1]] -= registers[memory[pc + 2]];
    }

    registers[PROGRAM_COUNTER] += 3;
  }
}


/*
  junk for tests
*/

int array_equal(int array1[], int array2[]) {
  for (int i = 0; i < 20; i++) {
    if (array1[i] != array2[i]) {
      return 0;
    }
  }
  return 1;
}


void print_array(int array[]) {
  printf("[ ");
  for (int i = 0; i < 20; i++) {
    printf("%x ", array[i]);
  }
  printf("]\n");
}

void test(int program[], int expected[]) {
  execute(program);
  if (array_equal(program, expected)) {
    printf("PASS\n");
  } else {
    printf("FAIL\n  actual:   ");
    print_array(program);
    printf("  expected: ");
    print_array(expected);
  }
}


int main(void) {
  int program1[20] = {
    LOAD_WORD, REG1, 0x10,
    LOAD_WORD, REG2, 0x12,
    ADD, REG1, REG2,
    STORE_WORD, REG1, 0x0e,
    HALT,
    BLANK,
    0x00, 0x00,
    0x01, 0x00,
    0x01, 0x00
  };
  int expected1[20] = {
    LOAD_WORD, REG1, 0x10,
    LOAD_WORD, REG2, 0x12,
    ADD, REG1, REG2,
    STORE_WORD, REG1, 0x0e,
    HALT,
    BLANK,
    0x02, 0x00,
    0x01, 0x00,
    0x01, 0x00
  };
  test(program1, expected1);

  int program2[20] = {
    LOAD_WORD, REG1, 0x10,
    LOAD_WORD, REG2, 0x12,
    ADD, REG1, REG2,
    STORE_WORD, REG1, 0x0e,
    HALT,
    BLANK,
    0x00, 0x00,
    0xff, 0x12,
    0x01, 0x01
  };
  int expected2[20] = {
    LOAD_WORD, REG1, 0x10,
    LOAD_WORD, REG2, 0x12,
    ADD, REG1, REG2,
    STORE_WORD, REG1, 0x0e,
    HALT,
    BLANK,
    0x00, 0x14,
    0xff, 0x12,
    0x01, 0x01
  };
  test(program2, program2);
}
