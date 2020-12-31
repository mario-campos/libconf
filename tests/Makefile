test: 
	$(CC) -I .. -o test_conf_next ../libconf.c test_conf_next.c -l atf-c
	$(CC) -I .. -o test_conf_accept ../libconf.c test_conf_accept.c -l atf-c
	$(CC) -I .. -o test_conf_eof ../libconf.c test_conf_eof.c -l atf-c
	$(CC) -I .. -o test_conf_expect ../libconf.c test_conf_expect.c -l atf-c
	$(CC) -I .. -o test_conf_string ../libconf.c test_conf_string.c -l atf-c
	kyua test

.PHONY: clean
clean:
	rm -f test_conf_next test_conf_accept test_conf_eof test_conf_expect test_conf_string
