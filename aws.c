#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int server_sockfd_tcp;
    int server_sockfd_udp;
    int client_sockfd;
    int len;
    struct sockaddr_in my_addr_tcp;   //AWS TCP
    struct sockaddr_in my_addr_udp;   //AWS UDP
    struct sockaddr_in remote_addr_client; //client
    struct sockaddr_in remote_addr_A; //server A
    struct sockaddr_in remote_addr_B; //server B
    struct sockaddr_in remote_addr_C; //server C
    int sin_size;
    char task[BUFSIZ];
    long int sendtoA[BUFSIZ];
    long int sendtoB[BUFSIZ];
    long int sendtoC[BUFSIZ];
    long int getfromA[10];
    long int getfromB[10];
    long int getfromC[10];
    long int datafromclient[BUFSIZ];
    long int result[10];
    
    memset(&task,0,sizeof(task));
    
    memset(&my_addr_tcp,0,sizeof(my_addr_tcp)); //clear tcp
    my_addr_tcp.sin_family=AF_INET;
    my_addr_tcp.sin_addr.s_addr=INADDR_ANY;
    my_addr_tcp.sin_port=htons(25874); //TCP port
    
    memset(&my_addr_udp,0,sizeof(my_addr_udp)); //clear udp
    my_addr_udp.sin_family=AF_INET;
    my_addr_udp.sin_addr.s_addr=INADDR_ANY;
    my_addr_udp.sin_port=htons(24874); //UDP port
    
    memset(&remote_addr_A,0,sizeof(remote_addr_A)); //server A
    remote_addr_A.sin_family=AF_INET;
    remote_addr_A.sin_addr.s_addr=inet_addr("127.0.0.1");
    remote_addr_A.sin_port=htons(21874); //ServerA port
    
    memset(&remote_addr_B,0,sizeof(remote_addr_B));//ServerB
    remote_addr_B.sin_family=AF_INET;
    remote_addr_B.sin_addr.s_addr=inet_addr("127.0.0.1");
    remote_addr_B.sin_port=htons(22874);
    
    memset(&remote_addr_C,0,sizeof(remote_addr_C)); //Server C
    remote_addr_C.sin_family=AF_INET;
    remote_addr_C.sin_addr.s_addr=inet_addr("127.0.0.1");
    remote_addr_C.sin_port=htons(23874);

    
    /*create a fd for tcp to client*/
    if((server_sockfd_tcp=socket(PF_INET,SOCK_STREAM,0))<0)
    {
        perror("create tcp socket error");
        return 1;
    }
    
    /*create a fd for udp*/
    if((server_sockfd_udp=socket(PF_INET,SOCK_DGRAM,0))<0)
    {
        perror("create udp socket error");
        return 1;
    }
    
    if (bind(server_sockfd_tcp,(struct sockaddr *)&my_addr_tcp,sizeof(struct sockaddr))<0)
    {
        perror("bindtcp");
        return 1;
    }
    
    if (bind(server_sockfd_udp,(struct sockaddr *)&my_addr_udp,sizeof(struct sockaddr))<0)
    {
        perror("bindudp");
        return 1;
    }
    
    /*listen length is 10*/
    listen(server_sockfd_tcp,10);
    
    sin_size=sizeof(struct sockaddr_in);
    
    printf("The AWS is up and running.\n");
    
    while (1) {
        /*wati for client*/
        if((client_sockfd=accept(server_sockfd_tcp,(struct sockaddr *)&remote_addr_client, &sin_size))<0)
        {
            perror("accept error");
            return 1;
        }
        
        /*receive the task from client and send it to A B C*/
        if ((len=recv(client_sockfd,task,BUFSIZ,0))>0)
        {
            int lengthoftask = len;
            task[lengthoftask] = '\0';
            
            //send the task to A
            if((len=sendto(server_sockfd_udp,task,strlen(task),0,(struct sockaddr *)&remote_addr_A,sizeof(struct sockaddr)))<0)
            {
                perror("send task to A error");
                return 1;
            }
            
            
            //send the task to B
            if((len=sendto(server_sockfd_udp,task,strlen(task),0,(struct sockaddr *)&remote_addr_B,sizeof(struct sockaddr)))<0)
            {
                perror("send task to B error");
                return 1;
            }
            
            
            //send the task to C
            if((len=sendto(server_sockfd_udp,task,strlen(task),0,(struct sockaddr *)&remote_addr_C,sizeof(struct sockaddr)))<0)
            {
                perror("send task to C error");
                return 1;
            }
            
        }
        memset(datafromclient, 0, sizeof(datafromclient));
        //receive data from client and send them to A B C
        len = recv(client_sockfd, datafromclient, BUFSIZ*8, 0);
        
        if (len > 0){
            
            long int total_number = datafromclient[0];
            
            //obtain the tcp port dynamiclly
            struct sockaddr_in mydynamicaddtcp;
            memset(&mydynamicaddtcp, 0, sizeof(mydynamicaddtcp));
            int sizedytcp = sizeof(mydynamicaddtcp);
            getsockname(server_sockfd_tcp, (struct sockaddr *) &mydynamicaddtcp , (socklen_t *) &sizedytcp);
            int AWS_tcp_port = ntohs(mydynamicaddtcp.sin_port);

            
            printf("The AWS has received %ld numbers from the client using TCP over port %d.\n", total_number, AWS_tcp_port);
            
            sendtoA[0] = total_number/3;
            sendtoB[0] = total_number/3;
            sendtoC[0] = total_number/3;
            int i = 1;
            for(i = 1; i <= total_number/3; i++){
                sendtoA[i] = datafromclient[i];
            }
            i = 1;
            for(i = 1; i <= total_number/3; i++){
                sendtoB[i] = datafromclient[total_number/3+i];
            }
            i = 1;
            for(i = 1; i <= total_number/3; i++){
                sendtoC[i] = datafromclient[2*total_number/3+i];
            }
            
            
            //send the data to A
            if((len=sendto(server_sockfd_udp,sendtoA,sizeof(sendtoA),0,(struct sockaddr *)&remote_addr_A,sizeof(struct sockaddr)))<0)
            {
                perror("send data to A error");
                return 1;
            }
            
            printf("The AWS sent %ld numbers to Backend­Server A.\n",sendtoA[0]);
            
            //send the data to B
            if((len=sendto(server_sockfd_udp,sendtoB,sizeof(sendtoB),0,(struct sockaddr *)&remote_addr_B,sizeof(struct sockaddr)))<0)
            {
                perror("send data to B error");
                return 1;
            }
            
             printf("The AWS sent %ld numbers to Backend­Server B.\n",sendtoB[0]);
            
            //send the data to C
            if((len=sendto(server_sockfd_udp,sendtoC,sizeof(sendtoC),0,(struct sockaddr *)&remote_addr_C,sizeof(struct sockaddr)))<0)
            {
                perror("send data to C error");
                return 1;
            }
            
            printf("The AWS sent %ld numbers to Backend­Server C.\n",sendtoC[0]);
            
        }
        //obtain the udp port dynamiclly
        struct sockaddr_in mydynamicadd;
        memset(&mydynamicadd, 0, sizeof(mydynamicadd));
        int sizedy = sizeof(mydynamicadd);
        getsockname(server_sockfd_udp, (struct sockaddr *) &mydynamicadd , (socklen_t *) &sizedy);
        int AWS_udp_port = ntohs(mydynamicadd.sin_port);
        
        //receive result from A
        if((len=recvfrom(server_sockfd_udp,getfromA,sizeof(getfromA),0,(struct sockaddr *)&remote_addr_A,&sin_size))<0)
        {
            perror("recvfrom");
            return 1;
        }
        printf("The AWS received reduction result of %s from Backend­Server A using UDP over port %d and it is %ld.\n",task, AWS_udp_port, getfromA[0]);
        
        //receive result from B
        if((len=recvfrom(server_sockfd_udp,getfromB,sizeof(getfromB),0,(struct sockaddr *)&remote_addr_B,&sin_size))<0)
        {
            perror("recvfrom");
            return 1;
        }
        printf("The AWS received reduction result of %s from Backend­Server B using UDP over port %d and it is %ld.\n",task, AWS_udp_port, getfromB[0]);
        
        //receive result from C
        if((len=recvfrom(server_sockfd_udp,getfromC,sizeof(getfromC),0,(struct sockaddr *)&remote_addr_C,&sin_size))<0)
        {
            perror("recvfrom");
            return 1;
        }
        printf("The AWS received reduction result of %s from Backend­Server C using UDP over port %d and it is %ld.\n",task, AWS_udp_port,getfromC[0]);
        
        //calculate
        long int max = 0;
        long int min = 0;
        if(strcmp(task,"sum") == 0){
            result[0] =getfromA[0]+getfromB[0]+getfromC[0];
        }
        if(strcmp(task,"sos") == 0){
            result[0] =getfromA[0]+getfromB[0]+getfromC[0];
        }
        if(strcmp(task,"max") == 0){
            max = getfromA[0];
            if(getfromB[0] > max){
                max = getfromB[0];
            }
            if(getfromC[0] > max){
                max = getfromC[0];
            }
            result[0] = max;
        }
        if(strcmp(task,"min") == 0){
            max = getfromA[0];
            if(getfromB[0] < max){
                max = getfromB[0];
            }
            if(getfromC[0] < max){
                max = getfromC[0];
            }
            result[0] = max;
        }
        
        printf("The AWS has successfully finished the reduction %s : %ld .\n",task, result[0]);
        
        //send the result to client
        if(send(client_sockfd,result,sizeof(result),0)<0)
        {
            perror("sent result to client error.");
            return 1;
        }
        
        printf("The AWS has successfully finished sending the reduction value to client.\n");
        
    }
    close(client_sockfd);
    close(server_sockfd_tcp);
    close(server_sockfd_udp);
    return 0;
}
