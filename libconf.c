#include <setjmp.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>

#include "libconf.h"

static void
skip_whitespace_and_comments(struct conf_state *cst)
{
	while (true) {
		if (cst->buf[cst->buf_index] == ' '
		 || cst->buf[cst->buf_index] == '\t')
			cst->buf_index++;
		else if (cst->buf[cst->buf_index] == '#') {
			while (true) {
				if (cst->buf[cst->buf_index] == '\n')
					break;
				if (cst->buf_index == cst->buf_size)
					break;
				cst->buf_index++;
			}
		}
		else break;
	}
}

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

const char *
conf_strerror(struct conf_state *cst)
{
	static char err_msg_buf[100] = {0};
	strlcpy(err_msg_buf, cst->err_text, sizeof(err_msg_buf));
	return err_msg_buf;
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
	skip_whitespace_and_comments(cst);
	return feof(cst->file) && cst->buf_index == cst->buf_size;
}

size_t
conf_next(struct conf_state *cst, char buf[], size_t buf_size)
{
	skip_whitespace_and_comments(cst);

	size_t n = 0;
	while (true) {
		if (cst->buf[cst->buf_index + n] == ' ' || cst->buf[cst->buf_index + n] == '\t' || cst->buf[cst->buf_index + n] == '\n')
			break;
		if (n == (buf_size-1))
			break;
		n++;
	}
	strlcpy(buf, &cst->buf[cst->buf_index], (n+1) < buf_size ? (n+1) : buf_size);
	return n;
}

size_t
conf_expect(struct conf_state *cst, const char *s)
{
	skip_whitespace_and_comments(cst);

	if (strncmp(&cst->buf[cst->buf_index], s, strlen(s)))
		conf_error(cst, LIBCONF_ERR_UNEXPECTED_TOKEN, "unexpected token");

	cst->buf_index += strlen(s);
	return strlen(s);
}

size_t
conf_accept(struct conf_state *cst, const char *s)
{
	skip_whitespace_and_comments(cst);

	if (strncmp(&cst->buf[cst->buf_index], s, strlen(s)))
		return 0;

	cst->buf_index += strlen(s);
	return strlen(s);
}

size_t
conf_string(struct conf_state *cst, char *buf, size_t size)
{
	skip_whitespace_and_comments(cst);

	bool in_double_quote = false;
	size_t n = 0;
	while (true) {
		if ((cst->buf_index + n) >= cst->buf_size)
			break;
		if ((n+1) == size)
			break;
		if (in_double_quote && cst->buf[cst->buf_index + n] == '"')
			break;
		if (!in_double_quote && cst->buf[cst->buf_index + n] == '"') {
			in_double_quote = true;
			cst->buf_index++;
		}
		if (!in_double_quote && cst->buf[cst->buf_index + n] == ' ' || cst->buf[cst->buf_index + n] == '\t')
			break;
		if (cst->buf[cst->buf_index + n] == '\n')
			break;
		n++;
	}

	strlcpy(buf, &cst->buf[cst->buf_index], (n+1) < size ? (n+1) : size);
	cst->buf_index += n + in_double_quote;

	return n;
}

size_t
conf_expect_re(struct conf_state *cst, const char *pattern)
{
	skip_whitespace_and_comments(cst);

	int re_err;
	regex_t re;
	regmatch_t re_loc[1];

	if (regcomp(&re, pattern, REG_EXTENDED)) {
		char message[100];
		regerror(re_err, &re, message, sizeof(message));
		char *err_msg = strdup(message);
		conf_error(cst, LIBCONF_ERR_REGEX, err_msg);
	}

	if ((re_err = regexec(&re, &cst->buf[cst->buf_index], 1, re_loc, 0))) {
		char message[100];
		regerror(re_err, &re, message, sizeof(message));
		char *err_msg = strdup(message);
		conf_error(cst, LIBCONF_ERR_REGEX, err_msg);
	}

	if (re_loc[0].rm_so != 0)
		conf_error(cst, LIBCONF_ERR_UNEXPECTED_TOKEN, strdup("unexpected token"));

	cst->buf_index += re_loc[0].rm_eo;
	return re_loc[0].rm_eo;
}

size_t
conf_accept_re(struct conf_state *cst, const char *pattern)
{
	skip_whitespace_and_comments(cst);

	int re_err;
	regex_t re;
	regmatch_t re_loc[1];

	if (regcomp(&re, pattern, REG_EXTENDED)) {
		char message[100];
		regerror(re_err, &re, message, sizeof(message));
		char *err_msg = strdup(message);
		conf_error(cst, LIBCONF_ERR_REGEX, err_msg);
	}

	re_err = regexec(&re, &cst->buf[cst->buf_index], 1, re_loc, 0);
	if (re_err == REG_NOMATCH || re_loc[0].rm_so != 0)
		return 0;
	else if (re_err) {
		char message[100];
		regerror(re_err, &re, message, sizeof(message));
		char *err_msg = strdup(message);
		conf_error(cst, LIBCONF_ERR_REGEX, err_msg);
	}

	cst->buf_index += re_loc[0].rm_eo;
	return re_loc[0].rm_eo;
}
