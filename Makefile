all: aws.c serverA.c serverB.c serverC.c client.c
	gcc -o aws_zy aws.c -lsocket -lnsl -lresolv
	gcc -o serverA_zy serverA.c -lsocket -lnsl -lresolv
	gcc -o serverB_zy serverB.c -lsocket -lnsl -lresolv
	gcc -o serverC_zy serverC.c -lsocket -lnsl -lresolv
	gcc -o client client.c -lsocket -lnsl -lresolv

aws:
	./aws_zy
serverA:
	./serverA_zy
serverB:
	./serverB_zy
serverC:
	./serverC_zy
