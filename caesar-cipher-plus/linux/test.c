#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "caesar.h"

int main(int argc, char *argv[])
{
	int i, j;
	int opt;
	uint32_t flags = 0, basic_flags = 0;
	char *text;
	char password[MAX_PSWD_LEN + 1];
	
	while ((opt = getopt(argc, argv, "edp:s:v")) != -1) {
		switch (opt) {
		case 'e':
			flags |= OP_ENCRYPT;
			break;
		case 'd':
			flags |= OP_DECRYPT;
			break;
		case 'p':
			flags |= PARAM_PSWD;
			if (strlen(optarg) > MAX_PSWD_LEN) {
				printf("password exceed %d characters\n", MAX_PSWD_LEN);
				exit(EXIT_FAILURE);
			}
			strncpy(password, optarg, CHAR_NUM - 1);
			break;
		case 's':
			flags |= PARAM_STR;
			text = malloc(strlen(optarg) + 1);
			strcpy(text, optarg);
			break;
		case 'v':
			flags |= PARAM_VERBOSE;
			break;
		default:
			printf("Unsupported Arguments!\n");
			exit(EXIT_FAILURE);
		}
	}

	basic_flags = PARAM_PSWD | PARAM_STR;
	if (((flags & (basic_flags | OP_ENCRYPT)) !=  (basic_flags | OP_ENCRYPT)) &&
	                ((flags & (basic_flags | OP_DECRYPT)) != (flags & (basic_flags | OP_DECRYPT)))) {
		printf("Usage: \
                        \n encrypt: caesar -e -p password -s string_to_encrypt \
                        \n decrypt: caesar -d -p password -s string_to_decrypt\n");
		exit(-1);
	}

	/* Remove duplicate character in password to generate key */
	if (init_env(password) != 0) {
		exit(EXIT_FAILURE);
	}

	if (flags & PARAM_VERBOSE) {
		printf("password: %s\n", password);
	}

	printf("string to process: %s\n", text);


	/* Encrypt */
	if (flags & OP_ENCRYPT) {
		encrypt(text);
		printf("encrypted: %s\n", text);
	}

	/* Decrypt */
	if (flags & OP_DECRYPT) {
		decrypt(text);
		printf("decrypted: %s\n", text);
	}

	if (flags & PARAM_VERBOSE) {
		printf("mapping table:\n");
		dump_mapping_table();
	}
}
