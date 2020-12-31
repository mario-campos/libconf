#include <atf-c.h>
#include "libconf.h"

#define STR_SIZE(x) (x),(sizeof(x)-1)

void tc0_parser(struct conf_ctx *ctx, void *arg)
{
	conf_error(ctx, -1, "error");
	ATF_CHECK(false);
}

ATF_TC_WITHOUT_HEAD(tc0);
ATF_TC_BODY(tc0, tc)
{
	struct conf_ctx ctx;
	FILE *fp = fmemopen(STR_SIZE("foo"), "r");
	conf_parse(&ctx, tc0_parser, fp, NULL);
	ATF_CHECK(true);
}

ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, tc0);
	return atf_no_error();
}
