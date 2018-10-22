#include <cstdio>
#include <cstdint>

#ifndef MAX_MEMO
#define MAX_MEMO 1024
#endif

#ifndef DEFAULT_BLOCK_SIZE
#define DEFAULT_BLOCK_SIZE 32
#endif

#define DATA_START_OFFSET 8

#define DEFAULT_INPUT ("input.bin")
#define DEFAULT_OUTPUT ("output.bin")

void transpose_block(uint8_t *block, size_t B)
{
	size_t source;
	size_t target;
	for (size_t i = 0; i < B; i++) {
		for (size_t j = i; j < B; j++) {
			source = i * B + j;
			target = j * B + i;
			if (source == target) {
				continue;
			}
			block[source] ^= block[target];
			block[target] ^= block[source];
			block[source] ^= block[target];
		}
	}
}

void transpose_thick(FILE *in, FILE *out)
{
	uint32_t dims[2];
	uint32_t swapped[2];
	size_t read_rows = 0;
	size_t read_cols = 0;
	size_t B = DEFAULT_BLOCK_SIZE;
	uint8_t *block = new uint8_t[B * B];

	fseek(in, 0, SEEK_SET);
	fread(dims, sizeof *dims, 2, in);
	swapped[0] = dims[1];
	swapped[1] = dims[0];
	fseek(out, 0, SEEK_SET);
	fwrite(swapped, sizeof *swapped, 2, out);
	for (size_t i = 0; i * B < dims[0]; i++) {
		for (size_t j = 0; j * B < dims[1]; j++) {
			for (size_t i1 = 0; i1 < B; i1++) {
				fseek(in, DATA_START_OFFSET + (i * B + i1) * dims[1] + j * B, SEEK_SET);
				fread(block + i1 * B, sizeof *block, B, in);
			}
			transpose_block(block, B);
			for (size_t i1 = 0; i1 < B; i1++) {
				fseek(out, DATA_START_OFFSET + (j * B + i1) * dims[0] + i * B, SEEK_SET);
				fwrite(block + i1 * B, sizeof *block, B, out);
			}
		}
	}

	delete[] block;
}

int main()
{
	FILE *in = fopen(DEFAULT_INPUT, "rb");
	FILE *out = fopen(DEFAULT_OUTPUT, "wb");

	transpose_thick(in, out);
	fcloseall();
	return 0;
}