#include <atf-c.h>
#include "libconf.h"

#define STR_SIZE(x) (x),(sizeof(x)-1)

void tc0_parser(struct conf_ctx *ctx, void *arg)
{
	return;
}

ATF_TC_WITHOUT_HEAD(tc0);
ATF_TC_BODY(tc0, tc)
{
	struct conf_ctx ctx;
	FILE *fp = fmemopen(STR_SIZE(""), "r");
	ATF_CHECK(conf_parse(&ctx, tc0_parser, fp, NULL));
}

void tc1_parser(struct conf_ctx *ctx, void *arg)
{
	conf_expect(ctx, "bar");
}

ATF_TC_WITHOUT_HEAD(tc1);
ATF_TC_BODY(tc1, tc)
{
	struct conf_ctx ctx;
	FILE *fp = fmemopen(STR_SIZE("foo"), "r");
	ATF_CHECK(!conf_parse(&ctx, tc1_parser, fp, NULL));
}

ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, tc0);
	ATF_TP_ADD_TC(tp, tc1);
	return atf_no_error();
}
