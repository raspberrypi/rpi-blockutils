#include <fcntl.h>
#include <limits.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#include <sparse/sparse.h>

/* Usage,
 *   blockmap_img2simg <BLOCK_SIZE> <BLOCK_MAP> <IMG> <SIMG>
 */

bool is_zeroes(char *data, unsigned len) {
	for (unsigned i=0; i < len; i++) {
		if (data[i]) return false;
	}
	return true;
}

int main(int argc, char *argv[]) {
	if (argc != 5) {
		fprintf(stderr, "Incorrect usage\n");
		return 1;
	}

	unsigned int bs;
	long int block_size = strtol(argv[1], NULL, 0);
	if (block_size > 0 && block_size < UINT_MAX && !(block_size & 3)) {
		bs = block_size;
	} else {
		fprintf(stderr, "Invalid block size, must be a multiple of 4\n");
		return 1;
	}

	FILE *bm = fopen(argv[2], "r");
	if (!bm) {
		fprintf(stderr, "Can't read block map\n");
		return 2;
	}

	int img = open(argv[3], O_RDONLY);
	if (img == -1) {
		fprintf(stderr, "Can't read image\n");
		return 3;
	}
	struct stat s;
	if (fstat(img, &s)) {
		fprintf(stderr, "Can't determine image size\n");
		return 4;
	}
	char *data = mmap(NULL, s.st_size, PROT_READ, MAP_PRIVATE, img, 0);
	if (!data) {
		fprintf(stderr, "Can't memory map image\n");
		return 5;
	}

	int out = creat(argv[4], S_IRUSR | S_IWUSR);
	if (out == -1) {
		fprintf(stderr, "Can't create output file\n");
		return 6;
	}

	struct sparse_file *sparse = sparse_file_new(bs, s.st_size);
	if (!sparse) {
		fprintf(stderr, "Can't create sparse cookie\n");
		return 7;
	}

	// Do stuff!!!
	unsigned int block = 0;
	while (s.st_size) {
		char valid_block;
		if (1 != fread(&valid_block, sizeof(valid_block), 1, bm)) {
			fprintf(stderr, "Unable to read block %u from block map\n", block);
			return 8;
		}

		int amount_data = s.st_size < bs ? s.st_size : bs;

		if (valid_block == '1') {
			if (is_zeroes(data + block * bs, bs)) {
				sparse_file_add_fill(sparse, 0, bs, block);
			} else {
				sparse_file_add_data(sparse, data + block * bs, bs, block);
			}
		}

		s.st_size -= amount_data;
		block++;
	}
	
	return sparse_file_write(sparse, out, false, true, false);
}

