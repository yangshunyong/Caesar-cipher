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
#include "caesar.h"

char mapping_table[MAP_TALBE_LEN];
char key[MAX_PSWD_LEN + 1];

static int get_password(char *password)
{
	scanf("%s", password);

	return strlen(password);
}

static int char_in_array(char ch, char *array, int len)
{
	int i;

	for (i = 0; i < len; i++) {
		if (array[i] == ch) {
			return i;
		}
	}

	return -1;
}

static void __encrypt(char *str, char char_start, char * mapping_table)
{
	int i;
	int len = strlen(str);

	for (i = 0; i < len; i++)
		str[i] = mapping_table[str[i] - char_start];
}

static void __decrypt(char *str, char char_start, char * mapping_table, int table_len)
{
	int i;
	int str_len = strlen(str);

	for (i = 0; i < str_len; i++)
		str[i] = char_start + char_in_array(str[i], mapping_table, table_len);
}


static void build_mapping_table(char *key, char *mapping_table, int table_len)
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

static void remove_duplicate_char(const char *src, char *dest)
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

static int check_key(const char *str) {
    while (*str != '\0') {
        if (*str < CHAR_START || *str > CHAR_END) {
            return -1;
        }
        str++;
    }
    return 0;
}

void __dump_mapping_table(char char_start, char *mapping_table, int table_len)
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

void dump_key(void)
{
	printf("key: %s\n", key);
}

void dump_mapping_table(void)
{
	__dump_mapping_table(CHAR_START, mapping_table, MAP_TALBE_LEN);
}

int encrypt(char *str) {
	__encrypt(str, CHAR_START, mapping_table);
	return 0;
}

int decrypt(char *str) {
	__decrypt(str, CHAR_START, mapping_table, MAP_TALBE_LEN);
	return 0;
}

int init_env(const char *passwd)
{
	if (strlen(passwd) > MAX_PSWD_LEN) {
		printf("password exceed %d characters\n", MAX_PSWD_LEN);
		return -1;
	}

	remove_duplicate_char(passwd, key);

	if (check_key(key) < 0) {
		printf("key contains invalid character\n");
		return -1;
	}

	/* Init mapping_table by key */
	memset(mapping_table, 0, MAP_TALBE_LEN);
	build_mapping_table(key, mapping_table, MAP_TALBE_LEN);

	return 0;
}