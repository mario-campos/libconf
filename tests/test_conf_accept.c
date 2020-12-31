#include <atf-c.h>
#include "libconf.h"

#define STR_SIZE(x) (x),(sizeof(x)-1)

void tc0_parser(struct conf_ctx *ctx, void *arg)
{
	ATF_CHECK_EQ(conf_accept(ctx, "xxx"), 3);
}

ATF_TC_WITHOUT_HEAD(tc0);
ATF_TC_BODY(tc0, tc)
{
	struct conf_ctx ctx;
	FILE *fp = fmemopen(STR_SIZE("xxx"), "r");
	conf_parse(&ctx, tc0_parser, fp, NULL);
}

void tc1_parser(struct conf_ctx *ctx, void *arg)
{
	ATF_CHECK_EQ(conf_accept(ctx, "xxx"), 0);
}

ATF_TC_WITHOUT_HEAD(tc1);
ATF_TC_BODY(tc1, tc)
{
	struct conf_ctx ctx;
	FILE *fp = fmemopen(STR_SIZE("yyy"), "r");
	conf_parse(&ctx, tc1_parser, fp, NULL);
}

void tc2_parser(struct conf_ctx *ctx, void *arg)
{
	conf_accept(ctx, "abc");
	ATF_CHECK_EQ(conf_accept(ctx, "abc"), 0);
}

ATF_TC_WITHOUT_HEAD(tc2);
ATF_TC_BODY(tc2, tc)
{
	struct conf_ctx ctx;
	FILE *fp = fmemopen(STR_SIZE("abc"), "r");
	conf_parse(&ctx, tc2_parser, fp, NULL);
}

ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, tc0);
	ATF_TP_ADD_TC(tp, tc1);
	ATF_TP_ADD_TC(tp, tc2);
	return atf_no_error();
}
