#include <cstdio>
#include <cstdint>

#ifndef MAX_MEMO
#define MAX_MEMO 1024
#endif

#ifndef DEFAULT_BLOCK_SIZE
#define DEFAULT_BLOCK_SIZE 512
#endif

#define DATA_START_OFFSET 8

#define DEFAULT_INPUT ("input.bin")
#define DEFAULT_OUTPUT ("output.bin")

size_t min(size_t x, size_t y) {
	return (x < y) ? x : y;
}

size_t max(size_t x, size_t y) {
	return (x > y) ? x : y;
}

void transpose_block(uint8_t *block, size_t B)
{
	size_t source;
	size_t target;
	for (size_t i = 0; i < B; i++) {
		for (size_t j = 0; j < i; j++) {
			source = i * B + j;
			target = j * B + i;
			block[source] ^= block[target];
			block[target] ^= block[source];
			block[source] ^= block[target];
		}
	}
}

void transpose_block(uint8_t *block, uint8_t *transposed, size_t m, size_t n)
{
	for (size_t i = 0; i < m; i++) {
		for (size_t j = 0; j < n; j++) {
			transposed[j * m + i] = block[i * n + j];
		}
	}
}

void transpose_fat(FILE *in, FILE *out, size_t m, size_t n)
{
	size_t B = DEFAULT_BLOCK_SIZE;
	uint8_t *block = new uint8_t[B * B];

	for (size_t i = 0; i * B < m; i++) {
		size_t src_rows = ((i + 1) * B <= m) ? B : m % B;
		for (size_t j = 0; j * B < n; j++) {
			size_t src_cols = ((j + 1) * B <= n) ? B : n % B;
			for (size_t i1 = 0; i1 < src_rows; i1++) {
				fseek(in, DATA_START_OFFSET + (i * B + i1) * n + j * B, SEEK_SET);
				fread(block + i1 * B, sizeof *block, src_cols, in);
			}
			transpose_block(block, B);
			for (size_t i1 = 0; i1 < src_cols; i1++) {
				fseek(out, DATA_START_OFFSET + (j * B + i1) * m + i * B, SEEK_SET);
				fwrite(block + i1 * B, sizeof *block, src_rows, out);
			}
		}
	}
	delete[] block;
}

void transpose_tall(FILE *in, FILE *out, size_t m, size_t n) {
	size_t B = DEFAULT_BLOCK_SIZE;
	uint8_t *source = new uint8_t[B * n];
	uint8_t *target = new uint8_t[n * B];

	fseek(in, DATA_START_OFFSET, SEEK_SET);
	for (size_t i = 0; i * B < m; i++) {
		size_t src_rows = ((i + 1) * B <= m) ? B : (m % B);
		fread(source, 1, n * src_rows, in);
		transpose_block(source, target, src_rows, n);
		for (size_t j = 0; j < n; j++) {
			fseek(out, DATA_START_OFFSET + j * m + i * B, SEEK_SET);
			fwrite(target + j * src_rows, 1, src_rows, out);
		}
	}
	delete[] source;
	delete[] target;
}

void transpose_long(FILE *in, FILE *out, size_t m, size_t n) {
	size_t B = DEFAULT_BLOCK_SIZE;
	uint8_t *source = new uint8_t[B * m];
	uint8_t *target = new uint8_t[m * B];

	fseek(out, DATA_START_OFFSET, SEEK_SET);
	for (size_t i = 0; i * B < n; i++) {
		size_t src_cols = ((i + 1) * B <= n) ? B : (n % B);
		for (size_t j = 0; j < m; j++) {
			fseek(in, DATA_START_OFFSET + j * n + i * B, SEEK_SET);
			fread(source + j * src_cols, 1, src_cols, in);
		}
		transpose_block(source, target, m, src_cols);
		fwrite(target, 1, m * src_cols, out);
	}
	delete[] source;
	delete[] target;
}

void transpose_sneaky(FILE *in, FILE *out, size_t m, size_t n) {
	size_t B = DEFAULT_BLOCK_SIZE;
	size_t data_len = m * n;
	uint8_t *block = new uint8_t[B * B];

	fseek(in, DATA_START_OFFSET, SEEK_SET);
	fseek(out, DATA_START_OFFSET, SEEK_SET);
	for (size_t i = 0; i * B * B < data_len; i++) {
		size_t block_size = ((i + 1) * B * B <= data_len) ? (B * B) : data_len % (B * B);

		fread(block, 1, block_size, in);
		fwrite(block, 1, block_size, out);
	}
	delete[] block;
}

void transpose(FILE *in, FILE *out)
{
	uint32_t dims[2];

	fseek(in, 0, SEEK_SET);
	fread(dims, sizeof *dims, 2, in);
	dims[0] ^= dims[1];
	dims[1] ^= dims[0];
	dims[0] ^= dims[1];
	fseek(out, 0, SEEK_SET);
	fwrite(dims, sizeof *dims, 2, out);

	if ((dims[0] == 1) || (dims[1] == 1)) {
		transpose_sneaky(in, out, dims[0], dims[1]);
	} else if ((2 * dims[0]) < DEFAULT_BLOCK_SIZE) {
		transpose_tall(in, out, dims[1], dims[0]);
	} else if ((2 * dims[1]) < DEFAULT_BLOCK_SIZE) {
		transpose_long(in, out, dims[1], dims[0]);
	} else {
		transpose_fat(in, out, dims[1], dims[0]);
	}
}

int main()
{
	FILE *in = fopen(DEFAULT_INPUT, "rb");
	FILE *out = fopen(DEFAULT_OUTPUT, "wb");

	transpose(in, out);
	fcloseall();
	return 0;
}