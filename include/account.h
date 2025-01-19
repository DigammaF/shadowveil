
#ifndef ACCOUNT_H
#define ACCOUNT_H

#define ADMIN (1 << 0)
#define BANNED (1 << 1)

typedef struct {
	char* name;
	char* password;
	unsigned flags;
	unsigned address;
} account_t;

#endif
