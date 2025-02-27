#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sparse/sparse.h>
#include <unistd.h>

_Static_assert(sizeof(int) <= sizeof(void *));

static int write_cbk(void *priv, const void *data, size_t len) {
	int fd = (intptr_t)priv;
	if (!data) {
		return (lseek(fd, len, SEEK_CUR) == -1) ? -1 : 0;
	} else {
		// TODO: Handle partial writes
		return (write(fd, data, len) == len) ? 0 : -1;
	}
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fputs("Usage: apply_simg <sparse image> <disk image>", stderr);
		return EXIT_FAILURE;
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "Failed to open sparse image: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	struct sparse_file *file = sparse_file_import(fd, true, false);
	if (!file) {
		fputs("Failed to read sparse file", stderr);
		return EXIT_FAILURE;
	}

	// TODO: Worth adding an error check for length?
	//int64_t len = sparse_file_len(file, false, false);

	int out_file = open(argv[2], O_WRONLY);
	if (!out_file) {
		fprintf(stderr, "Failed to open output file: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	int ret = sparse_file_callback(file, false, false, write_cbk, (void *)((intptr_t)out_file));

	close(fd);
	close(out_file);

	sparse_file_destroy(file);

	return (ret == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
