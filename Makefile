run: killServer compileServer compileClient runProgram

compileServer:
	-gcc PrimaryLikesServer.c -o server

compileClient:
	-gcc ParentProcess.c -o client

runProgram:
	-./server & ./client

clearLogs:
	-rm /tmp/LikeServer0
	-rm /tmp/LikeServer1
	-rm /tmp/LikeServer2
	-rm /tmp/LikeServer3
	-rm /tmp/LikeServer4
	-rm /tmp/LikeServer5
	-rm /tmp/LikeServer6
	-rm /tmp/LikeServer7
	-rm /tmp/LikeServer8
	-rm /tmp/LikeServer9
	-rm /tmp/ParentProcessStatus
	-rm /tmp/PrimaryLikesLog

killServer:
	-killall server