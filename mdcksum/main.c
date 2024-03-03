#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#define MD_CODE_START 0x200

int md_cksum(char *s, size_t length)
{
	int checksum = 0;
	size_t i;

	for(i = MD_CODE_START; i < length; i += 2) {
		checksum += ((s[i] << 8) + s[i+1]) & 0xffff;
		//printf("%x\n", checksum);
	}

	return checksum & 0xffff;
}

static size_t get_file_len(FILE *fp)
{
        size_t len;
        int pos;

        /* store current position */
        pos = ftell(fp);
        fseek(fp, 0L, SEEK_END);
        len = ftell(fp);
        /* restore to original position */
        fseek(fp, 0L, pos);

        return len;
}

static void usage(const char *name, int code)
{
        printf("usage: %s FILE\n", name);
        exit(code);
}

int main(int argc, char **argv)
{
        FILE *fp;
        char *file_name, *contents;
        size_t i, file_length;
	int c;

	if(argc < 2)
		usage(argv[0], EXIT_FAILURE);

	file_name = argv[1];

        fp = fopen(file_name, "rb");
        if(!fp) {
		perror("fopen");
                return EXIT_FAILURE;
        }

        file_length = get_file_len(fp);
        if(file_length == 0)
                fprintf(stderr, "warning: file is 0 bytes long\n");

        contents = malloc(file_length * sizeof(char) + 1);
        for(i = 0; i < file_length; i++) {
		c = fgetc(fp);
		if(c == EOF) {
			fprintf(stderr, "warning: unexpected EOF at %lu encountered.\n", 
					i);
			exit(EXIT_FAILURE);
		}
                contents[i] = c;
	}
        assert(fgetc(fp) == EOF && i == file_length);
        fclose(fp);

	printf("%04x\n", md_cksum(contents, file_length));

        free(contents);

        return EXIT_SUCCESS;
}
