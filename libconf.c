#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "libconf.h"

void
conf_init(struct conf_ctx *ctx)
{

}

bool
conf_parse(struct conf_ctx *ctx, conf_parser_t func, FILE *fp, void *arg)
{
	if (setjmp(ctx->jump) > 0)
		return false;
	ctx->file = fp;
	ctx->buf_size = fread(ctx->buf, sizeof(char), sizeof(ctx->buf), fp);
	ctx->buf_index = 0;
	func(ctx, arg);
}

bool
conf_eof(struct conf_ctx *ctx)
{
	return feof(ctx->file) && ctx->buf_index == ctx->buf_size;
}

bool
conf_next(struct conf_ctx *ctx, const char *s)
{
	return !strncmp(&ctx->buf[ctx->buf_index], s, strlen(s));
}

size_t
conf_expect(struct conf_ctx *ctx, const char *s)
{
	if (strncmp(&ctx->buf[ctx->buf_index], s, strlen(s)))
		longjmp(ctx->jump, 1);

	ctx->buf_index += strlen(s);
	return strlen(s);
}

size_t
conf_accept(struct conf_ctx *ctx, const char *s)
{
	if (strncmp(&ctx->buf[ctx->buf_index], s, strlen(s)))
		return 0;

	ctx->buf_index += strlen(s);
	return strlen(s);
}

size_t
conf_string(struct conf_ctx *ctx, char *buf, size_t size)
{
	// 1. determine the range of bytes to copy
	size_t n = 0;
	while (true) {
		if ((ctx->buf_index + n) >= ctx->buf_size)
			break;
		if (ctx->buf[ctx->buf_index + n] == ' ' || ctx->buf[ctx->buf_index + n] == '\t' || ctx->buf[ctx->buf_index + n] == '\n')
			break;
		n++;
	}

	// 2. copy those bytes
	memcpy(buf, &ctx->buf[ctx->buf_index], n);

	// 3. set null terminator
	buf[n] = '\0';

	// 4. update internal pointer
	ctx->buf_index += n;

	return n;
}
