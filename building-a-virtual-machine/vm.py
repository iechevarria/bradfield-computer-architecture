PROGRAM_COUNTER = 0x00

# instructions
LOAD_WORD = 0x01
STORE_WORD = 0x02
ADD = 0x03
SUB = 0x04
HALT = 0xff
BLANK = 0x00


def read_word(memory, index):
    return memory[index] + (256 * memory[index + 1])


def write_word(memory, index, value):
    memory[index] = value % 256
    memory[index + 1] = value // 256


def execute(memory):
    registers = [0, 0, 0]

    # 14 bytes of memory for instructions
    while registers[PROGRAM_COUNTER] < 14:

        # get current instruction, make my life easier
        pc = registers[PROGRAM_COUNTER]
        instruction = memory[pc]

        if instruction == HALT:
            break
        elif instruction == LOAD_WORD:
            registers[memory[pc + 1]] = read_word(
                memory=memory,
                index=memory[pc + 2],
            )
        elif instruction == STORE_WORD:
            write_word(
                memory=memory,
                index=memory[pc + 2],
                value=registers[memory[pc + 1]],
            )
        elif instruction == ADD:
            registers[memory[pc + 1]] += registers[memory[pc + 2]]
        elif instruction == SUB:
            registers[memory[pc + 1]] -= registers[memory[pc + 2]]
        else:
            print("ERROR: invalid instruction")

        # all instructions have 2 args, so offset by 3
        registers[PROGRAM_COUNTER] += 3



def test(program, expected):
    execute(program)
    if program == expected:
        print("PASS")
    else:
        print(f"FAIL\n  expected: {expected}\n  actual:   {program}")


if __name__ == '__main__':
    program1 = [
        LOAD_WORD, 0x01, 0x10,
        LOAD_WORD, 0x02, 0x12,
        ADD, 0x01, 0x02,
        STORE_WORD, 0x01, 0x0e,
        HALT,
        BLANK,
        0x00, 0x00,
        0x01, 0x00,
        0x01, 0x00,
    ]
    expected1 = [
        LOAD_WORD, 0x01, 0x10,
        LOAD_WORD, 0x02, 0x12,
        ADD, 0x01, 0x02,
        STORE_WORD, 0x01, 0x0e,
        HALT,
        BLANK,
        0x02, 0x00,
        0x01, 0x00,
        0x01, 0x00,
    ]

    test(program1, expected1)

    program2 = [
        LOAD_WORD, 0x01, 0x10,
        LOAD_WORD, 0x02, 0x12,
        ADD, 0x01, 0x02,
        STORE_WORD, 0x01, 0x0e,
        HALT,
        BLANK,
        0x00, 0x00,
        0xff, 0x12,
        0x01, 0x01,
    ]
    expected2 = [
        LOAD_WORD, 0x01, 0x10,
        LOAD_WORD, 0x02, 0x12,
        ADD, 0x01, 0x02,
        STORE_WORD, 0x01, 0x0e,
        HALT,
        BLANK,
        0x00, 0x14,
        0xff, 0x12,
        0x01, 0x01,
    ]

    test(program2, expected2)
