sv:
	gcc src/*.c -I include -D SERVER_TALK -D SERVER_REQUEST_LOG -o sv.out

debug:
	gcc src/*.c -I include -D SERVER_DEBUG -o sv_debug.out
	./sv_debug.out


debug2:
	gcc src/*.c -I include -D SERVER_DEBUG2 -o sv_debug.out
	./sv_debug.out
