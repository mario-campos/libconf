#ifndef LIBCONF_H
#define LIBCONF_H

#include <setjmp.h>
#include <stdio.h>
#include <stdbool.h>

#define LIBCONF_SUCCESS 		0x0000
#define LIBCONF_ERR_EOF			0x0001
#define LIBCONF_ERR_UNEXPECTED_TOKEN 	0x0002
#define LIBCONF_ERR_READ		0x0003
#define LIBCONF_ERR_REGEX		0x0004

struct conf_state {
	jmp_buf jump;
	FILE *file;
	char buf[1024];
	size_t buf_index;
	size_t buf_size;
	int err_code;
	const char *err_text;
};

void	conf_init(struct conf_state *);
bool	conf_parse(struct conf_state *, void (*)(struct conf_state *, void *), FILE *, void *);

bool	conf_eof(struct conf_state *);
void	conf_error(struct conf_state *, int, const char *);
size_t	conf_next(struct conf_state *, char[], size_t);
size_t	conf_expect(struct conf_state *, const char *);
size_t	conf_accept(struct conf_state *, const char *);
size_t	conf_string(struct conf_state *, char[], size_t);
size_t	conf_expect_re(struct conf_state *, const char *);
size_t	conf_accept_re(struct conf_state *, const char *);

const char *conf_strerror(struct conf_state *);

#endif /* LIBCONF_H */
