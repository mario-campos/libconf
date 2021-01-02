#include <atf-c.h>
#include "libconf.h"

#define STR_SIZE(x) (x),(sizeof(x)-1)

void tc0_parser(struct conf_state *cst, void *arg)
{
	ATF_CHECK(conf_next(cst, "foo"));
}

ATF_TC_WITHOUT_HEAD(tc0);
ATF_TC_BODY(tc0, tc)
{
	struct conf_state cst;
	FILE *fp = fmemopen(STR_SIZE("foo"), "r");
	conf_parse(&cst, tc0_parser, fp, NULL);
}

void tc1_parser(struct conf_state *cst, void *arg)
{
	ATF_CHECK(!conf_next(cst, "foo"));
}

ATF_TC_WITHOUT_HEAD(tc1);
ATF_TC_BODY(tc1, tc)
{
	struct conf_state cst;
	FILE *fp = fmemopen(STR_SIZE("bar"), "r");
	conf_parse(&cst, tc1_parser, fp, NULL);
}

void tc2_parser(struct conf_state *cst, void *arg)
{
	ATF_CHECK(conf_next(cst, "xxx"));
}

ATF_TC_WITHOUT_HEAD(tc2);
ATF_TC_BODY(tc2, tc)
{
	struct conf_state cst;
	FILE *fp = fmemopen(STR_SIZE(" xxx"), "r");
	conf_parse(&cst, tc2_parser, fp, NULL);
}

void tc3_parser(struct conf_state *cst, void *arg)
{
	ATF_CHECK(conf_next(cst, "xxx"));
}

ATF_TC_WITHOUT_HEAD(tc3);
ATF_TC_BODY(tc3, tc)
{
	struct conf_state cst;
	FILE *fp = fmemopen(STR_SIZE("\t\txxx"), "r");
	conf_parse(&cst, tc3_parser, fp, NULL);
}

void tc4_parser(struct conf_state *cst, void *arg)
{
	ATF_CHECK(conf_next(cst, "\nfoo"));
}

ATF_TC_WITHOUT_HEAD(tc4);
ATF_TC_BODY(tc4, tc)
{
	struct conf_state cst;
	FILE *fp = fmemopen(STR_SIZE("# comment\nfoo"), "r");
	conf_parse(&cst, tc4_parser, fp, NULL);
}

void tc5_parser(struct conf_state *cst, void *arg)
{
	ATF_CHECK(conf_next(cst, "\nfoo"));
}

ATF_TC_WITHOUT_HEAD(tc5);
ATF_TC_BODY(tc5, tc)
{
	struct conf_state cst;
	FILE *fp = fmemopen(STR_SIZE("  \t \t\t# comment\nfoo"), "r");
	conf_parse(&cst, tc5_parser, fp, NULL);
}

ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, tc0);
	ATF_TP_ADD_TC(tp, tc1);
	ATF_TP_ADD_TC(tp, tc2);
	ATF_TP_ADD_TC(tp, tc3);
	ATF_TP_ADD_TC(tp, tc4);
	ATF_TP_ADD_TC(tp, tc5);
	return atf_no_error();
}
