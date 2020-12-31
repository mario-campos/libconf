#include <atf-c.h>
#include "libconf.h"

#define STR_SIZE(x) (x),(sizeof(x)-1)

void tc0_parser(struct conf_state *cst, void *arg)
{
	ATF_CHECK_EQ(conf_accept(cst, "xxx"), 3);
}

ATF_TC_WITHOUT_HEAD(tc0);
ATF_TC_BODY(tc0, tc)
{
	struct conf_state cst;
	FILE *fp = fmemopen(STR_SIZE("xxx"), "r");
	conf_parse(&cst, tc0_parser, fp, NULL);
}

void tc1_parser(struct conf_state *cst, void *arg)
{
	ATF_CHECK_EQ(conf_accept(cst, "xxx"), 0);
}

ATF_TC_WITHOUT_HEAD(tc1);
ATF_TC_BODY(tc1, tc)
{
	struct conf_state cst;
	FILE *fp = fmemopen(STR_SIZE("yyy"), "r");
	conf_parse(&cst, tc1_parser, fp, NULL);
}

void tc2_parser(struct conf_state *cst, void *arg)
{
	conf_accept(cst, "abc");
	ATF_CHECK(conf_eof(cst));
}

ATF_TC_WITHOUT_HEAD(tc2);
ATF_TC_BODY(tc2, tc)
{
	struct conf_state cst;
	FILE *fp = fmemopen(STR_SIZE("abc"), "r");
	conf_parse(&cst, tc2_parser, fp, NULL);
}

ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, tc0);
	ATF_TP_ADD_TC(tp, tc1);
	ATF_TP_ADD_TC(tp, tc2);
	return atf_no_error();
}
