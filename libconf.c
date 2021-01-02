#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "libconf.h"

void
conf_init(struct conf_state *cst)
{

}

bool
conf_parse(struct conf_state *cst, void (*func)(struct conf_state *, void *), FILE *fp, void *arg)
{
	if (setjmp(cst->jump) > 0)
		return false;
	cst->file = fp;
	cst->buf_size = fread(cst->buf, sizeof(char), sizeof(cst->buf), fp);
	cst->buf_index = 0;
	func(cst, arg);
	return true;
}

void
conf_error(struct conf_state *cst, int error_code, const char *message)
{
	cst->err_code = error_code;
	cst->err_text = message;
	longjmp(cst->jump, 1);
}

bool
conf_eof(struct conf_state *cst)
{
	while (true) {
		if (cst->buf[cst->buf_index] == ' '
		 || cst->buf[cst->buf_index] == '\t'
		 || cst->buf[cst->buf_index] == '\n')
			cst->buf_index++;
		else break;
	}

	return feof(cst->file) && cst->buf_index == cst->buf_size;
}

bool
conf_next(struct conf_state *cst, const char *s)
{
	while (true) {
		if (cst->buf[cst->buf_index] == ' '
		 || cst->buf[cst->buf_index] == '\t'
		 || cst->buf[cst->buf_index] == '\n')
			cst->buf_index++;
		else break;
	}

	return !strncmp(&cst->buf[cst->buf_index], s, strlen(s));
}

size_t
conf_expect(struct conf_state *cst, const char *s)
{
	while (true) {
		if (cst->buf[cst->buf_index] == ' '
		 || cst->buf[cst->buf_index] == '\t'
		 || cst->buf[cst->buf_index] == '\n')
			cst->buf_index++;
		else break;
	}

	if (strncmp(&cst->buf[cst->buf_index], s, strlen(s)))
		longjmp(cst->jump, 1);

	cst->buf_index += strlen(s);
	return strlen(s);
}

size_t
conf_accept(struct conf_state *cst, const char *s)
{
	while (true) {
		if (cst->buf[cst->buf_index] == ' '
		 || cst->buf[cst->buf_index] == '\t'
		 || cst->buf[cst->buf_index] == '\n')
			cst->buf_index++;
		else break;
	}

	if (strncmp(&cst->buf[cst->buf_index], s, strlen(s)))
		return 0;

	cst->buf_index += strlen(s);
	return strlen(s);
}

size_t
conf_string(struct conf_state *cst, char *buf, size_t size)
{
	// 0. ignore whitespace
	while (true) {
		if (cst->buf[cst->buf_index] == ' '
		 || cst->buf[cst->buf_index] == '\t'
		 || cst->buf[cst->buf_index] == '\n')
			cst->buf_index++;
		else break;
	}

	// 1. determine the range of bytes to copy
	size_t n = 0;
	while (true) {
		if ((cst->buf_index + n) >= cst->buf_size)
			break;
		if ((n+1) == size)
			break;
		if (cst->buf[cst->buf_index + n] == ' ' || cst->buf[cst->buf_index + n] == '\t' || cst->buf[cst->buf_index + n] == '\n')
			break;
		n++;
	}

	// 2. copy those bytes
	memcpy(buf, &cst->buf[cst->buf_index], n);

	// 3. set null terminator
	buf[n] = '\0';

	// 4. update internal pointer
	cst->buf_index += n;

	return n;
}
