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
	for (char *s = &ctx->buf[0]; *s == ' ' || *s == '\t'; s++);
	if (!conf_next(ctx, s))
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
	size_t index = ctx->buf_index;
	while (true) {
		char c = ctx->buf[index];
		if (c != ' ' && c != '\t' && c != '\n')
			index++;
		else
			break;
	}
	size_t diff = index - ctx->buf_index;
	ctx->buf_index = index;
	return diff;
}
