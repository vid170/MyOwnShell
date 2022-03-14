#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>


void cyan () {
  printf("\033[0;36m");
}
void white () {
  printf("\033[0m");
}
void yellow() {
  printf("\033[1;33m");
}

char* removeLeadingSpaces(char* str)
{
    static char str1[99];
    int count = 0, j, k;

    while (str[count] == ' ') {
        count++;
    }
  
    for (j = count, k = 0;
         str[j] != '\0'; j++, k++) {
        str1[k] = str[j];
    }
    str1[k] = '\0';
  
    return str1;
}
int runredirectcommand(char *mycommands, char* filen)
{
    char *pch;
        char *myargs[100];
        pch = strtok(mycommands, " ");
        myargs[0] = strdup(pch);
        int i = 1;
        while (pch != NULL)
        {
            pch = strtok(NULL, " ");
            if(pch!=NULL)
            {
            myargs[i] = strdup(pch);
            i++;
            }
            else
            break;
        }
        myargs[i]=NULL;
        int value2=strcmp(myargs[0],strdup("exit"));
     if(value2==0)
       {
           return 2;        
       }
        int value=strcmp(myargs[0],strdup("cd"));
       if(value==0)  
       {
       chdir(myargs[1]);
       return 0;
       } 
     int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {     char* f1=removeLeadingSpaces(filen);
          char * f2=strtok(f1," ");
          int fd = open(f2, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        //   printf("%s\n",f2);
                dup2(fd, 1);   // make stdout go to file
                execvp(myargs[0], myargs);
                close(fd);
                exit(0);             
    }
    else
    {
        int status;
            if (wait(&status) == -1)
            {
            perror("wait()");
            }
            else
            {
                if(WIFEXITED(status))
                {
                    return(WEXITSTATUS(status));
                }
                else if (WIFSIGNALED(status))
                {
                    return (WTERMSIG(status));
                }
            }
    }
}
int runprocess(char *str,int flag,int total,int curr,int count)
{       
        char *pch;
        char *mycommands3[100];
        pch = strtok(str, ">");
        mycommands3[0] = strdup(pch);
        pch = strtok(NULL, ">");
        int texit=0;
        if(pch!=NULL)
        {
        char *filen= strdup(pch);
        texit=runredirectcommand(mycommands3[0],filen);
        return texit;
        }
        else
        {
        char *pch;
        char *myargs[100];
        pch = strtok(str, " ");
        myargs[0] = strdup(pch);
        int i = 1;
        while (pch != NULL)
        {
            pch = strtok(NULL, " ");
            if(pch!=NULL)
            {
            myargs[i] = strdup(pch);
            i++;
            }
            else
            break;
        }
        myargs[i]=NULL;
        int value=strcmp(myargs[0],strdup("cd"));
        if(value==0)
        {
            chdir(myargs[1]); 
            return 0;       
        }
        int value2=strcmp(myargs[0],strdup("exit"));
        if(value2==0)
        {
            return 2;        
        }  
        int rc = fork();
        if (rc < 0)
        {
            fprintf(stderr, "fork failed\n");
            exit(1);
        }
        else if (rc == 0)
        {   
            if(flag==0)
                {
                    execvp(myargs[0], myargs);
                    printf("Command not found, PLease enter a valid command\n");  
                    exit(1);
                }
            else if(flag==1)
            {
                if(curr==0)
                {
                int fd = open("vidhi", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
                dup2(fd, 1);   // make stdout go to file
                execvp(myargs[0], myargs);
                close(fd);
                exit(0); 
                }
                else if(curr>0 && (total-curr)>1)
                {
                    if(count==1)
                    myargs[i]=strdup("vidhi");
                    else
                    myargs[i]=strdup("vidhi2");
                    i++;
                    myargs[i]=NULL;
                    char *filen;
                    if(count==1)
                    filen=strdup("vidhi2");
                    else
                    filen=strdup("vidhi");
                    int fd = open(filen, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
                    dup2(fd, 1);   // make stdout go to file
                    execvp(myargs[0], myargs);
                    close(fd);
                    exit(0);
                }
                else if((total-curr)==1)
                {
                    if(count==1)
                    myargs[i]=strdup("vidhi");
                    else
                    myargs[i]=strdup("vidhi2");
                    i++;
                    myargs[i]=NULL;
                    execvp(myargs[0],myargs);
                    printf("Command not found, PLease enter a valid command\n");
                    exit(1);
                }
            }  
        }
        else
        {
              int status;
            if (wait(&status) == -1)
            {
            perror("wait()");
            }
            else
            {
                if(WIFEXITED(status))
                {
                    return(WEXITSTATUS(status));
                }
                else if (WIFSIGNALED(status))
                {
                    return (WTERMSIG(status));
                }
            }
        } 
    return 0;
    }
}


int runprocessbigger(char *str)
{
        char *pch;
        char *mycommands2[100];
        int i=0;
        pch = strtok(str, "|");
        mycommands2[i]=pch;
        i++;
         while (pch != NULL)
        {
            pch = strtok(NULL, "|");
            if(pch!=NULL)
            {
                mycommands2[i]=pch;
                i++;

            }
            else
            break;
        }
        int texit=0;
        for(int j=0;j<i;j++)
        {
            // printf("%s\n",mycommands[j]);
            if(i>1)
            {   int p=0,count=0;
                if(j==0||j==1)
                {
                // int p=1;
                p=1;
                }
                else if(count>1 && count%2==0)
                p=0;
                else if(count%2==1)
                p=1;
                texit=runprocess(mycommands2[j],1,i,j,p);
            if(texit==2)
            {
                // flag=1;
                return 2;
            }
            count++;
            if((i-j)==1)
            return texit;
            
            }
            else
            {
                 texit=runprocess(mycommands2[j],0,i,j,0);
                if(texit==2)
                return 2;
                if((i-j)==1)
                return texit;
            }

        }
}



int main(int argc, char *argv[])
{
    
    while(1)
       {    remove("vidhi2");
            remove("vidhi");
            char s[100];
            char* c;
            size_t length,j;
            cyan();
            printf("Vidhi_Mittal@12041730:~");
            yellow();
            printf("%s#", getcwd(s, 100));
            white();
            char str[200];
           
            // scanf("%[^\n]%*c", str);
            if( fgets(str, sizeof(str), stdin) != NULL ) 
            {
            if(str[0]=='\n')
            {
                // printf("empty/");
                continue;
            }
            str[strcspn(str, "\n")] = 0;
        
            char *pch;
            char *mycommands[100];
            int i=0;
            pch = strtok(str, "&&");
            mycommands[i]=pch;
            i++;
            while (pch != NULL)
            {
                pch = strtok(NULL, "&&");
                if(pch!=NULL)
                {
                    mycommands[i]=pch;
                    i++;

                }
                else
                    break;
            }
            int texit=0;
            for(int j=0;j<i;j++)
            {   
                // printf("%s\n",mycommands[j]);
                texit=runprocessbigger(mycommands[j]);
                // printf("%d\n",texit);
                if(texit!=0)
                {
                    break;
                }
            }
            if(texit==2)
                break;      
            }
       }
    return 0;
}

