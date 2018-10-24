#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cassert>

#define INPUT_FILE_NAME ("input.bin")
#define OUTPUT_FILE_NAME ("output.expected.bin")

#define DEFAULT_BLOCK_SIZE 512

// usage: ./test_generator.out <rows_cnt> <cols_cnt>
int main(int argc, char *argv[])
{
    FILE *input_file = fopen(INPUT_FILE_NAME, "wb");
    FILE *output_file = fopen(OUTPUT_FILE_NAME, "wb");

    if (argc < 3) {
        fprintf(stderr, "usage: ./test_generator.out <rows_cnt> <cols_cnt>\n");
        exit(EXIT_FAILURE);
    }

    uint32_t m = atoi(argv[1]); // rows
    uint32_t n = atoi(argv[2]); // cols
    assert(fwrite(&m, 4, 1, input_file) == 1);
    assert(fwrite(&n, 4, 1, input_file) == 1);
    uint32_t source_counter = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            uint8_t element = UINT8_C(source_counter++);
            fseek(input_file, 8 + i * n + j, SEEK_SET);
            assert(fwrite(&element, 1, 1, input_file) == 1);
        }
    }

    assert(fwrite(&n, 1, 4, output_file) == 4);
    assert(fwrite(&m, 1, 4, output_file) == 4);
    uint32_t target_counter = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            uint8_t element = UINT8_C(target_counter++);
            fseek(output_file, 8 + j * m + i, SEEK_SET);
            assert(fwrite(&element, 1, 1, output_file) == 1);
        }
    }
    fcloseall();
    return 0;
}