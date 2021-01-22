#include <atf-c.h>
#include "libconf.h"

#define STR_SIZE(x) (x),(sizeof(x)-1)

void parse_string_foo(struct conf_state *cst, void *arg)
{
	char argbuf[10];
	conf_string(cst, argbuf, sizeof(argbuf));
	ATF_CHECK_EQ(strcmp(argbuf, "foo"), 0);
}


ATF_TC(conf_string_parses_until_space);
ATF_TC_HEAD(conf_string_parses_until_space, tc)
{
	atf_tc_set_md_var(tc, "descr",
	    "Test that conf_string() parses until the space character.");
}
ATF_TC_BODY(conf_string_parses_until_space, tc)
{
	struct conf_state cst;
	conf_parse(&cst, parse_string_foo, fmemopen(STR_SIZE("foo "), "r"), NULL);
}


ATF_TC(conf_string_parses_until_tab);
ATF_TC_HEAD(conf_string_parses_until_tab, tc)
{
	atf_tc_set_md_var(tc, "descr",
	    "Test that conf_string() parses until the tab character.");
}
ATF_TC_BODY(conf_string_parses_until_tab, tc)
{
	struct conf_state cst;
	conf_parse(&cst, parse_string_foo, fmemopen(STR_SIZE("foo\t"), "r"), NULL);
}


ATF_TC(conf_string_parses_until_linefeed);
ATF_TC_HEAD(conf_string_parses_until_linefeed, tc)
{
	atf_tc_set_md_var(tc, "descr",
	    "Test that conf_string() parses until the linefeed character.");
}
ATF_TC_BODY(conf_string_parses_until_linefeed, tc)
{
	struct conf_state cst;
	conf_parse(&cst, parse_string_foo, fmemopen(STR_SIZE("foo\n"), "r"), NULL);
}


ATF_TC(conf_string_ignores_whitespace_then_parses_until_EOF);
ATF_TC_HEAD(conf_string_ignores_whitespace_then_parses_until_EOF, tc)
{
	atf_tc_set_md_var(tc, "descr",
	    "Test that conf_string() ignores the leading whitespace, and then "
	    "parses until the end of the input.");
}
ATF_TC_BODY(conf_string_ignores_whitespace_then_parses_until_EOF, tc)
{
	struct conf_state cst;
	conf_parse(&cst, parse_string_foo, fmemopen(STR_SIZE("  \t \t\tfoo"), "r"), NULL);
}


ATF_TC(conf_string_ignores_whitespace_then_parses_until_space);
ATF_TC_HEAD(conf_string_ignores_whitespace_then_parses_until_space, tc)
{
	atf_tc_set_md_var(tc, "descr",
	    "Test that conf_string() ignores the leading whitespace, and then "
	    "parses until the space character.");
}
ATF_TC_BODY(conf_string_ignores_whitespace_then_parses_until_space, tc)
{
	struct conf_state cst;
	conf_parse(&cst, parse_string_foo, fmemopen(STR_SIZE("  \t \t\tfoo "), "r"), NULL);
}


ATF_TC(conf_string_ignores_whitespace_then_parses_until_tab);
ATF_TC_HEAD(conf_string_ignores_whitespace_then_parses_until_tab, tc)
{
	atf_tc_set_md_var(tc, "descr",
	    "Test that conf_string() ignores the leading whitespace, and then "
	    "parses until the tab character.");
}
ATF_TC_BODY(conf_string_ignores_whitespace_then_parses_until_tab, tc)
{
	struct conf_state cst;
	conf_parse(&cst, parse_string_foo, fmemopen(STR_SIZE("  \t \t\tfoo\t"), "r"), NULL);
}


ATF_TC(conf_string_ignores_whitespace_then_parses_until_linefeed);
ATF_TC_HEAD(conf_string_ignores_whitespace_then_parses_until_linefeed, tc)
{
	atf_tc_set_md_var(tc, "descr",
	    "Test that conf_string() ignores the leading whitespace, and then "
	    "parses until the linefeed character.");
}
ATF_TC_BODY(conf_string_ignores_whitespace_then_parses_until_linefeed, tc)
{
	struct conf_state cst;
	conf_parse(&cst, parse_string_foo, fmemopen(STR_SIZE("  \t \t\tfoo\n"), "r"), NULL);
}


void parse_into_buffer(struct conf_state *cst, void *arg)
{
	char buf[10];	
	ATF_CHECK_EQ(conf_string(cst, buf, sizeof(buf)), 9);
}


ATF_TC(conf_string_does_not_overflow_buffer);
ATF_TC_HEAD(conf_string_does_not_overflow_buffer, tc)
{
	atf_tc_set_md_var(tc, "descr",
	    "Test that conf_string() ignores the leading whitespace, parses the digits,"
	    " but does not overflow the buffer.");
}
ATF_TC_BODY(conf_string_does_not_overflow_buffer, tc)
{
	struct conf_state cst;
	conf_parse(&cst, parse_into_buffer, fmemopen(STR_SIZE("  \t \t\t01234567890"), "r"), NULL);
}


ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, conf_string_parses_until_space);
	ATF_TP_ADD_TC(tp, conf_string_parses_until_tab);
	ATF_TP_ADD_TC(tp, conf_string_parses_until_linefeed);
	ATF_TP_ADD_TC(tp, conf_string_ignores_whitespace_then_parses_until_EOF);
	ATF_TP_ADD_TC(tp, conf_string_ignores_whitespace_then_parses_until_space);
	ATF_TP_ADD_TC(tp, conf_string_ignores_whitespace_then_parses_until_tab);
	ATF_TP_ADD_TC(tp, conf_string_ignores_whitespace_then_parses_until_linefeed);
	ATF_TP_ADD_TC(tp, conf_string_does_not_overflow_buffer);
	return atf_no_error();
}
