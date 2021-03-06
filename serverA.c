//
//  main.c
//  Server_A
//
//  Created by 赵羿 on 11/16/16.
//  Copyright © 2016 Yi_Zhao. All rights reserved.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int serverA_sockfd;
    int len;
    struct sockaddr_in remote_addr; //remote is AWS
    struct sockaddr_in my_addr; //Server A
    int sin_size;
    char task[BUFSIZ];
    long int result[10];
    long int getdata[BUFSIZ];
    
    memset(&task,0,sizeof(task));
    
    memset(&remote_addr,0,sizeof(remote_addr)); //clear the data first
    remote_addr.sin_family=AF_INET;
    remote_addr.sin_addr.s_addr=inet_addr("127.0.0.1");//AWS
    remote_addr.sin_port=htons(24874); //AWS port
    
    memset(&my_addr,0,sizeof(my_addr));
    my_addr.sin_family=AF_INET;
    my_addr.sin_addr.s_addr=INADDR_ANY;//ServerA IP
    my_addr.sin_port=htons(21874); //ServerA port
    
    if((serverA_sockfd=socket(PF_INET,SOCK_DGRAM,0))<0)
    {
        perror("create socket error");
        return 1;
    }
    if (bind(serverA_sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))<0)
    {
        perror("create bind error");
        return 1;
    }
    
    sin_size=sizeof(struct sockaddr_in);
    
    //obtain the port dynamiclly
    struct sockaddr_in mydynamicadd;
    memset(&mydynamicadd, 0, sizeof(mydynamicadd));
    int sizedy = sizeof(mydynamicadd);
    getsockname(serverA_sockfd, (struct sockaddr *) &mydynamicadd , (socklen_t *) &sizedy);
    int port = ntohs(mydynamicadd.sin_port);
    
    printf("The Server A is up and running using UDP on port %d.\n",port);
    
    while(1){
        //receive the task and save it
        len=recvfrom(serverA_sockfd,task,BUFSIZ,0,(struct sockaddr *)&remote_addr,&sin_size);
        if(len<0)
        {
            perror("recvfrom task error");
            return 1;
        }
        task[len] = '\0';
        
        //receive the data and save it
        len=recvfrom(serverA_sockfd,getdata,sizeof(getdata),0,(struct sockaddr *)&remote_addr,&sin_size);
        if(len<0)
        {
            perror("recvfrom data error");
            return 1;
        }
        
        printf("The Server A has received %ld numbers.\n", getdata[0]);
        
        long int sum = 0;
        long int max = 0;
        long int min = 0;
        long int sos = 0;
        
        if(strcmp(task,"sum") == 0){
            
            long int totalnumber = getdata[0];
            
            int i;
            for(i = 1; i <= totalnumber; i++){
                sum = sum + getdata[i];
            }
            
            result[0] = sum;
            
            printf("The Server A has successfully finished the reduction %s: %ld.\n", task, result[0]);
            
            if((len=sendto(serverA_sockfd,result,sizeof(result),0,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr)))<0)
            {
                perror("recvfrom");
                return 1;
            }
            
            printf("The Server A has successfully finished sending the reduction value to AWS server.\n");
            
        }
        if(strcmp(task,"max") == 0){
            int totalnumber = getdata[0];
            max = getdata[1];
            int i;
            for(i = 1; i <= totalnumber; i++){
                if(getdata[i]>max){
                    max = getdata[i];
                }
            }
            result[0] = max;
            
            printf("The Server A has successfully finished the reduction %s: %ld.\n", task, result[0]);
            
            if((len=sendto(serverA_sockfd,result,sizeof(result),0,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr)))<0)
            {
                perror("recvfrom");
                return 1;
            }
            
            printf("The Server A has successfully finished sending the reduction value to AWS server.\n");
        }
        if(strcmp(task,"min") == 0){
            int totalnumber = getdata[0];
            min = getdata[1];
            int i = 1;
            for(i = 1; i <= totalnumber; i++){
                if(getdata[i] < min){
                    min = getdata[i];
                }
            }
            result[0] = min;
            
            printf("The Server A has successfully finished the reduction %s: %ld.\n", task, result[0]);
            
            if((len=sendto(serverA_sockfd,result,sizeof(result),0,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr)))<0)
            {
                perror("recvfrom");
                return 1;
            }
            
            printf("The Server A has successfully finished sending the reduction value to AWS server.\n");
            
        }
        if(strcmp(task,"sos") == 0){
            int totalnumber = getdata[0];
            int i = 1;
            for(i = 1; i <= totalnumber; i++){
                sos = sos + getdata[i]*getdata[i];
            }
            result[0] = sos;
            
            printf("The Server A has successfully finished the reduction %s: %ld.\n", task, result[0]);
            
            if((len=sendto(serverA_sockfd,result,sizeof(result),0,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr)))<0)
            {
                perror("recvfrom");
                return 1;
            }
            
            printf("The Server A has successfully finished sending the reduction value to AWS server.\n");
        }
        

    }
    close(serverA_sockfd);
    return 0;
}

