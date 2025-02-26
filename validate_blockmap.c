#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/* Usage,
 *   validate_blockmap <BLOCK_SIZE> <BLOCK_MAP> <IMG>
 */

int main(int argc, char *argv[]) {
	if (argc != 4) return 1;

	unsigned int bs;
	long int block_size = strtol(argv[1], NULL, 0);
	if (block_size > 0 && block_size < UINT_MAX && !(block_size & 3)) {
		bs = block_size;
	} else {
		fprintf(stderr, "Invalid block size, must be a multiple of 4\n");
		return 1;
	}
	FILE *map = fopen(argv[2], "r");
	FILE *data = fopen(argv[3], "rb");

	if (!data || !map) return 2;

	char *block = malloc(bs);
	if (!block) return 2;
	char valid;

	size_t ret;
	size_t iter = 0;
	while (EOF != (ret = fread(block, bs, 1, data))) {
		if (ret != 1) {
			fprintf(stderr, "fread returned %lu on iter %lu\n", ret, iter);
			return 3;
		}
		if (1 != fread(&valid, 1, 1, map)) {
			fprintf(stderr, "Ran out of blocks in blockmap on iter %lu\n", iter);
			return 4;
		}

		if (valid == '1') goto skip;

		for (size_t i=0; i < bs; i++) {
			if (block[i]) {
				fprintf(stderr, "Block %lu marked invalid but contains data\n", iter);
				goto skip;
			}
		}

skip:
		iter++;
	}

	return 0;
}
