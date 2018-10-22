#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cassert>

#define INPUT_FILE_NAME ("input.bin")
#define OUTPUT_FILE_NAME ("output.expected.bin")

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
    assert(fwrite(&m, 1, 4, input_file) == 4);
    assert(fwrite(&n, 1, 4, input_file) == 4);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            uint8_t element = UINT8_C(i * m + j);
            assert(fwrite(&element, 1, 1, input_file) == 1);
        }
    }

    assert(fwrite(&n, 1, 4, output_file) == 4);
    assert(fwrite(&m, 1, 4, output_file) == 4);
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < m; i++) {
            uint8_t element = UINT8_C(i * m + j);
            assert(fwrite(&element, 1, 1, output_file) == 1);
        }
    }
    fcloseall();
    return 0;
}