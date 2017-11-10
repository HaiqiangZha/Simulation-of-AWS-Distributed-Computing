# Simulation-of-AWS-Distributed-Computing
EE450
ABOUT THIS ASSIGNMENT:

In this project, we define a client to read the data from file nums.csv and get the task to do with
these data. When client get the task and data, then send the task as char[] and the data as long
[] to the AWS as the main server. When the server AWS get the task then send the task to three 
backend server A B C. After sending the task, the client send the data, after AWS get the data, it 
divides them into three long[] and forward them to A B C. When the backend server get the task 
and the data. It do the responsive calculation of these data. And return the result to AWS. AWS 
calculates three results from A B C and  return the final result to client.
The communication between client and AWS is TCP connection. And the communication between
AWS and three backend servers is UDP connection.
________________________ 

________________________

CODE FILES
client.c     read the data from mums.csv file and the task from input argument
was.c       receive the task and data from client.c and forward them to three
                backend servers. Receive the calculation result from A B C and calculate 
                the final result to client.
serverA.c get the data and task from AWS and send the result to AWS
serverB.c get the data and task from AWS and send the result to AWS
serverC.c get the data and task from AWS and send the result to AWS
Makefile
________________________

________________________

HOW TO RUN
Compile:          Type   make all
Run:                 Type   make serverA
                         Type   make serverB
		   Type   make serverC 
                         Type  make aws
                         Type  ./client <function_name>
__________________________

__________________________

EXCHANGED MESSAGE:

client:
The client is up and running.
The client has sent the reduction <function_name>  to AWS. 
The client has sent <count> numbers to AWS
The client has received reduction sum: <count>
AWS:
The AWS is up and running. 
The AWS has received <count> numbers from the client using TCP over port <port number> 
The AWS sent <count> numbers to Backend­Server<i>. 
The AWS received reduction result of <reduction type> from Backend­Server <i> using UDP over port <port number> and it is Xi 
The AWS has successfully finished the reduction <reduction type>: Xf inal 
The AWS has successfully finished sending the reduction value to client. 
server<i>:
The Server <i> is up and running using UDP on port <port number>. 
The Server <i> has received <count> numbers 
The Server <i> has successfully finished the reduction <reduction type>: Xi 
The Server <i> has successfully finished sending the reduction value to AWS server. 
_____________________________
_____________________________
IDIOSYNCRASY
Almostly, it always works successfully, however, it failed only once due to the mistake in 
the transmission in TCP socket. I do not know the reason, but after the second day, 
it works well. If it fails in the test, please run it again, it will work succesfully.
