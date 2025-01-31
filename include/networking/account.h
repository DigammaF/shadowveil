
#ifndef ACCOUNT_H
#define ACCOUNT_H

#define ADMIN_FLAG (1 << 0)
#define BANNED_FLAG (1 << 1)

typedef struct {
	char* name;
	char* password;
	unsigned flags;
	unsigned address;
} account_t;

#endif
