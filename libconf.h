#ifndef LIBCONF_H
#define LIBCONF_H

#include <setjmp.h>
#include <stdio.h>
#include <stdbool.h>

#define LIBCONF_SUCCESS 		0x0001
#define LIBCONF_ERR_EOF			0x0002
#define LIBCONF_ERR_UNEXPECTED_TOKEN 	0x0002
#define LIBCONF_ERR_READ		0x0003

struct conf_ctx {
	jmp_buf jump;
	FILE *file;
	char buf[1024];
	size_t buf_index;
	size_t buf_size;
};

typedef void (*conf_parser_t)(struct conf_ctx *ctx, void *arg);

void conf_init(struct conf_ctx *);
bool conf_parse(struct conf_ctx *, conf_parser_t, FILE *, void *);

bool conf_eof(struct conf_ctx *);
bool conf_next(struct conf_ctx *, const char *);
size_t conf_expect(struct conf_ctx *, const char *);
size_t conf_accept(struct conf_ctx *, const char *);
size_t conf_string(struct conf_ctx *, char *, size_t);

#endif /* LIBCONF_H */
