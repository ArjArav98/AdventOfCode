#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Register { A = 0, B = 1, C = 2 };
enum Opcode { ADV = 0, BXL = 1, BST = 2, JNZ = 3, BXC = 4, OUT = 5, BDV = 6, CDV = 7 };

struct Computer {
    long registers[3];

    long* program;
    size_t program_length;
    size_t instruction_pointer;
};
void computer_read_state_from_file(FILE* const input_file,
                                   struct Computer* const computer);
long computer_combo_operand(const struct Computer* const computer, const long operand);
long computer_execute_instruction(struct Computer* const computer);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

void computer_read_state_from_file(FILE* const input_file,
                                   struct Computer* const computer) {
    const size_t LINE_LEN = 50, REG_NUM_POS = 12, PROG_NUMS_POS = 9;
    char* const line = malloc(LINE_LEN * sizeof *line);

    fgets(line, LINE_LEN, input_file);
    computer->registers[A] = strtol(line + REG_NUM_POS, NULL, 10);
    fgets(line, LINE_LEN, input_file);
    computer->registers[B] = strtol(line + REG_NUM_POS, NULL, 10);
    fgets(line, LINE_LEN, input_file);
    computer->registers[C] = strtol(line + REG_NUM_POS, NULL, 10);

    fgets(line, LINE_LEN, input_file);
    fgets(line, LINE_LEN, input_file);

    computer->program_length = ((strlen(line) - PROG_NUMS_POS) + 1) / 2;
    computer->program = calloc(computer->program_length, sizeof computer->program);

    for (size_t i = 0; i < computer->program_length; i++) {
        computer->program[i] = strtol(line + PROG_NUMS_POS + (i * 2), NULL, 10);
    }

    free(line);
}

long computer_combo_operand(const struct Computer* const computer, const long operand) {
    if ((operand >= 0) && (operand <= 3)) return operand;
    else if ((operand >= 4) && (operand <= 6)) return computer->registers[operand - 4];
    else return -1;
}

long computer_execute_instruction(struct Computer* const computer) {
    const enum Opcode opcode = computer->program[computer->instruction_pointer];
    const long operand = computer->program[computer->instruction_pointer + 1];

    long output = -1;
    switch (opcode) {
        case ADV: {
            const long denominator =
                (long)pow(2, computer_combo_operand(computer, operand));
            computer->registers[A] = (computer->registers[A] / denominator);
            break;
        }
        case BXL:
            computer->registers[B] = computer->registers[B] ^ operand;
            break;
        case BST:
            computer->registers[B] = computer_combo_operand(computer, operand) % 8;
            break;
        case JNZ:
            if (computer->registers[A] != 0)
                computer->instruction_pointer = operand - 2;
            break;
        case BXC:
            computer->registers[B] = computer->registers[B] ^ computer->registers[C];
            break;
        case OUT:
            output = computer_combo_operand(computer, operand) % 8;
            break;
        case BDV: {
            const long denominator =
                (long)pow(2, computer_combo_operand(computer, operand));
            computer->registers[B] = (computer->registers[A] / denominator);
            break;
        }
        case CDV: {
            const long denominator =
                (long)pow(2, computer_combo_operand(computer, operand));
            computer->registers[C] = (computer->registers[A] / denominator);
            break;
        }
    }

    computer->instruction_pointer += 2;
    return output;
}

int main(void) {
    FILE* const input_file = fopen("./input.txt", "r");

    struct Computer computer = {.registers = {0, 0, 0},
                                .program = NULL,
                                .program_length = 0,
                                .instruction_pointer = 0};

    computer_read_state_from_file(input_file, &computer);

    while (computer.instruction_pointer < computer.program_length) {
        const long output = computer_execute_instruction(&computer);
        if (output != -1) {
            printf("%ld,", output);
        }
    }
    printf(" (ignore the last comma)\n");

    free(computer.program);
    fclose(input_file);
    return EXIT_SUCCESS;
}
