#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int client_sockfd;
    int len;
    struct sockaddr_in remote_addr; //AWS
    char task[BUFSIZ];
    char temp[BUFSIZ];
    long int result[10];

    
    long int readdata[BUFSIZ];
    
    memset(readdata, 0, sizeof(readdata));
    
    FILE *inputfile = fopen("./nums.csv", "r");
    int totalnumbercount = 0;
    
    
    while (NULL != fgets(temp, BUFSIZ, inputfile)) {
        totalnumbercount++;
        readdata[totalnumbercount] = atoi(temp);
    }
    readdata[0] = totalnumbercount;
    
    memset(&remote_addr,0,sizeof(remote_addr)); //Clear the AWS address
    remote_addr.sin_family=AF_INET; //set the IP communication
    remote_addr.sin_addr.s_addr=inet_addr("127.0.0.1");//set the AWS IP
    remote_addr.sin_port=htons(25874); //Set the AWS port
    
    /*create a fd for TCP*/
    if((client_sockfd=socket(PF_INET,SOCK_STREAM,0))<0)
    {
        perror("create socket error");
        return 1;
    }
    
    /*combine the fd to the AWS*/
    if(connect(client_sockfd,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr))<0)
    {
        perror("create connect error");
        return 1;
    }
    
    printf("The client is up and running.\n");
    int i = 0;
    
    for(i = 0; i < 3; i++){
        task[i] = argv[1][i];
    }
    task[3] = '\0';
    
    //send the task
    len=send(client_sockfd,task,strlen(task),0);
    if (len >= 0){
        printf("The client has sent the reduction type %s to AWS.\n", task);
    }
    
    //send the data
    len=send(client_sockfd,readdata,sizeof(readdata),0);
    if (len >= 0){
        printf("The client has sent %ld numbers to AWS.\n", readdata[0]);
    }
    
    //receive the result
    len=recv(client_sockfd,result,sizeof(result),0);
    if (len >= 0){
        printf("The client has received reduction %s: %ld.\n", task, result[0]);
    }
    
    close(client_sockfd);//close the fd
    return 0;
}
