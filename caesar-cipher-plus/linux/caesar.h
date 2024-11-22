#ifndef __CAESAR_H__
#define __CAESAR_H__

#define DUMP_ROW_WIDTH (15)
#define MAX_PSWD_LEN (255)

#define CHAR_START (0x21) /* '!'*/
#define CHAR_END   (0x7E)  /* '~' */
#define CHAR_NUM   (CHAR_END - CHAR_START + 1)
#define MAP_TALBE_LEN (CHAR_NUM)

enum {
	OP_ENCRYPT    = 0x01,
	OP_DECRYPT    = 0x02,
	PARAM_PSWD    = 0x04,
	PARAM_STR     = 0x08,
	PARAM_VERBOSE = 0x10,
};



int init_env(const char *passwd);
int decrypt(char *str);
int encrypt(char *str);
void dump_key(void);
void dump_mapping_table(void);

#endif