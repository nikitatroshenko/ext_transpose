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

    size_t B = DEFAULT_BLOCK_SIZE;
    uint8_t *block = new uint8_t[B * B];
    int index = 0;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            block[index++] = UINT8_C(i * n + j);
            if (index == B * B) {
                index = 0;
                fwrite(block, 1, B * B, input_file);
            }
        }
    }
    if (index != 0) {
        fwrite(block, 1, index, input_file);
    }

    fwrite(&n, 4, 1, output_file);
    fwrite(&m, 4, 1, output_file);
    index = 0;
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < m; i++) {
            block[index++] = UINT8_C(i * n + j);
            if (index == B * B) {
                index = 0;
                fwrite(block, 1, B * B, output_file);
            }
        }
    }
    if (index != 0) {
        fwrite(block, 1, index, output_file);
    }

    fcloseall();
    return 0;
}