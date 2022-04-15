/*
 * MIT License
 *
 * Copyright (c) 2022 Shunyong Yang (yang.shunyong@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DUMP_ROW_WIDTH (15)
#define MAX_PSWD_LEN (255)

#define CHAR_START (0x21) /* '!'*/
#define CHAR_END   (0x7E)  /* '~' */
#define CHAR_NUM   (CHAR_END - CHAR_START + 1)

enum {
	OP_ENCRYPT    = 0x01,
	OP_DECRYPT    = 0x02,
	PARAM_PSWD    = 0x04,
	PARAM_STR     = 0x08,
	PARAM_VERBOSE = 0x10,
};

char mapping_table[CHAR_NUM];

int get_password(char *password)
{
	scanf("%s", password);

	return strlen(password);
}

int char_in_array(char ch, char *array, int len)
{
	int i;

	for (i = 0; i < len; i++) {
		if (array[i] == ch) {
			return i;
		}
	}

	return -1;
}

void encrypt(char *str, char char_start, char * mapping_table)
{
	int i;
	int len = strlen(str);

	for (i = 0; i < len; i++)
		str[i] = mapping_table[str[i] - char_start];
}

void decrypt(char *str, char char_start, char * mapping_table, int table_len)
{
	int i;
	int str_len = strlen(str);

	for (i = 0; i < str_len; i++)
		str[i] = char_start + char_in_array(str[i], mapping_table, table_len);
}


void build_mapping_table(char *key, char *mapping_table, int table_len)
{
	int i, j;
	int idx;
	int pass_len;
	char candidate;

	pass_len = strlen(key);
	memcpy(mapping_table, key, pass_len);
	idx = pass_len;

	candidate = ((mapping_table[pass_len - 1] + 1 - CHAR_START) % CHAR_NUM) + CHAR_START;

	for (i = pass_len; i <CHAR_NUM; i++) {
		for (j = CHAR_START; j <= CHAR_END; j++) {
			if (char_in_array(candidate, mapping_table, table_len) < 0)
				break;
			else
				candidate = ((candidate + 1 - CHAR_START) % CHAR_NUM) + CHAR_START;
		}
		mapping_table[i] = candidate;
		candidate = ((candidate + 1 - CHAR_START) % CHAR_NUM) + CHAR_START;
	}
}

void dump_mapping_table(char char_start, char *mapping_table, int table_len)
{
	int i, width;
	int idx[3] = {};
	int len = table_len;

	while (len > 0) {
		width = (len > DUMP_ROW_WIDTH) ? DUMP_ROW_WIDTH : len;
		len -= width;

		printf("|ASCII ");
		for (i = 0; i < width; i++) {
			printf("|%3d", CHAR_START + idx[0]);
			idx[0]++;
		}
		printf("|\n");
		printf("|------");
		for (i = 0; i < width; i++) {
			printf("|---");
		}
		printf("|\n");
		printf("|plain ");
		for (i = 0; i < width; i++) {
			printf("| %c ", CHAR_START + idx[1]);
			idx[1]++;
		}
		printf("|\n");
		printf("|cipher");
		for (i = 0; i < width; i++) {
			printf("| %c ", mapping_table[idx[2]]);
			idx[2]++;
		}
		printf("|\n");

		printf("\n");
	}
}

void remove_duplicate_char(char *src, char *dest)
{
	char counter[255] = {};

	while(*src != '\0') {
		if (counter[*src] == 0) {
			counter[*src] = 1;
			*dest = *src;
			dest++;
		}
		src++;
	}

	*dest = '\0';
}

int main(int argc, char *argv[])
{
	int i, j;
	int opt;
	int table_len = sizeof(mapping_table);
	uint32_t flags = 0, basic_flags = 0;
	char *text;
	char password[MAX_PSWD_LEN + 1];
	char key[MAX_PSWD_LEN + 1];

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
	remove_duplicate_char(password, key);

	if (flags & PARAM_VERBOSE) {
		printf("password: %s\n", password);
		printf("key:      %s\n", key);
	}

	printf("string to process: %s\n", text);
	/* Init mapping_table by key */
	memset(mapping_table, 0, table_len);
	build_mapping_table(key, mapping_table, table_len);

	/* Encrypt */
	if (flags & OP_ENCRYPT) {
		encrypt(text, CHAR_START, mapping_table);
		printf("encrypted: %s\n", text);
	}

	/* Decrypt */
	if (flags & OP_DECRYPT) {
		decrypt(text, CHAR_START, mapping_table, table_len);
		printf("decrypted: %s\n", text);
	}

	if (flags & PARAM_VERBOSE) {
		printf("mapping table:\n");
		dump_mapping_table(CHAR_START, mapping_table, table_len);
	}
}
