all:
	gcc -g -Wall -o serverAoutput serverA.c
	gcc -g -Wall -o serverBoutput serverB.c
	gcc -g -Wall -o serverCoutput serverC.c -lm
	gcc -g -Wall -o awsoutput aws.c
	gcc -g -Wall -o monitoroutput monitor.c
	gcc -g -Wall -o client client.c

serverA: serverAoutput
	@./serverAoutput

serverB: serverBoutput
	@./serverBoutput

serverC: serverCoutput
	@./serverCoutput

aws: awsoutput
	@./awsoutput

monitor: monitoroutput
	@./monitoroutput
clean:
	rm -rf serverAoutput serverBoutput serverCoutput awsoutput monitoroutput client
