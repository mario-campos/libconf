test: 
	$(CC) -I .. -o test_conf_next ../libconf.c test_conf_next.c -l atf-c
	$(CC) -I .. -o test_conf_accept ../libconf.c test_conf_accept.c -l atf-c
	kyua test
