// C Program to design a shell in Linux 
#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>

#include<sys/types.h> 
#include<sys/wait.h> 
#include<readline/readline.h> 
#include<readline/history.h>

#define max1 1000
int var=1;


int pipe_split_new(char *cmd )
{
	int i=0;
	int a=0;
	for(;i<strlen(cmd);i++)
	{
	  if(cmd[i]=='|')
	    a++;


	}
	return a;
}

int pipe_split(char *cmd, char** pipeparsed)
{
	int i=0;
	for(;i<max1;i++)
	{
	pipeparsed[i]=strsep(&cmd,"|");
	
	if(pipeparsed[i]==NULL)
	break;


	}
	return (i-1);
}

void parse_space(char *cmd, char **parsed)
{

	
  int i=0;
  for(;i<max1;i++)
    {
      parsed[i]=strsep(&cmd," ");

      if(parsed[i]==NULL)
	break;
      if(strlen(parsed[i])==0)
	 i--;
    }
  int a=strlen(parsed[i-1]);
	
  /*  char *temp;
   temp=parsed[i-1];
   parsed[i-1]=strsep(&temp,"\n");
   if(strlen(parsed[i-1])==0)
     {
       parsed[i-1]=parsed[i];
       parsed[i]=NULL;
     }*/
  
  
  
}
void making_exec(char *cmd,char **newcmd1)
{
 char *newcmd=malloc(sizeof(char)*1000);

*newcmd1=newcmd;
 int len=strlen(cmd);
 int i=0;
 if(cmd[i]!='.' && cmd[i]!='/')
   {
     newcmd[0]='.';
     newcmd[1]='/';
     while(cmd[i]!=' ' && i<len)
       {
	 newcmd[i+2]=cmd[i];
	 i++;
       }
     
   }
 else
   strcpy(newcmd,cmd);
return;
}


void starting_line()
{
  printf("shell>>");
}
void pwd()
{
  char string[max1];
  getcwd(string,max1);
 
  printf("%s\n",string);
 
  return;
}

void exec_cmd(char **parsed)
{
  pid_t pid=fork();
  if(pid<0)
    printf("Forking failed");
  else if(pid==0)
    {
      execvp(parsed[0],parsed);

      printf("There is a error\n");
      exit(0);

    }
  wait(NULL);
  return;
}
void piped_new(char *cmd,char *input,char *output,int a)
{

  // printf("ABSADS12");
  int tmpin=dup(0);
  int tmpout=dup(1);
  int fdin;
  int fdout;
  char *pipeparsed[max1];

   	
	for(int i=0;i<max1;i++)
	{
	pipeparsed[i]=strsep(&cmd,"|");
	
	if(pipeparsed[i]==NULL)
	break;


	}

  
     if(strlen(input)!=0)
    {
      fdin=open(input,O_RDONLY);
    }
    else
      {  fdin=dup(tmpin);}
 
  int ret;
 
    for(int i=0;i<a+1;i++)
    {
   
       dup2(fdin,0);
      close(fdin);

      if(i==a)
	{
	 
	   if(strlen(output)!=0)
	    {
      fdout= open (output,O_RDWR | O_CREAT );
      chmod(output,S_IRWXU);
	    }
	  else
	  {
	    fdout=dup(tmpout);
	    }
	      
	  
	}
      else
	{
	  int fdpipe[2];
	  pipe(fdpipe);
	  fdout=fdpipe[1];
	  fdin=fdpipe[0];
	}
      dup2(fdout,1);
      close(fdout);
      
    
       ret=fork();
     
       if(ret==0)
	{
	
	  char *tempcmd[max1];
	  parse_space(pipeparsed[i],tempcmd);
	  char *newexec1;
        
	  making_exec(tempcmd[0],&newexec1);
	  execvp(newexec1,tempcmd);
	  
	    
	 _exit(1);
	}
   }
  
     dup2(tmpin,0);
  dup2(tmpout,1);
  close(tmpin);
  close(tmpout);

  waitpid(ret,NULL,0);

}



void without_piped(char** parsed , char *input, char *output)
{
  pid_t pid1,pid2;
    int fd[2];

    if(pipe(fd)==-1)
      {
	printf("Could Not pipe/n");
	return ;
	
      }
    pid1=fork();
    if(pid1<0)
      {
	 printf("Forking failed");
	 return;
	 }
    int f1,f2;
    int a=strlen(input);
    int b=strlen(output);
    //  printf("hello\n");
    
    	

			// printf("\nthe value ofa is %d and b is %d",a,b);
		if(a!=0)
     f1=open(input,O_RDONLY);
    if(b!=0)
      {
	f2=open(output,O_RDWR | O_CREAT );
	chmod(output,S_IRWXU);
	}
      if(pid1==0)
      {
		//Child process
	if(strlen(input)!=0)
	  {
	    close(0);
		//	dup2(f1,fd[0]);
       	dup(f1);
	  }

		if(strlen(output)!=0)
	  {
		
		close(1);
		//	dup2(f2,fd[1]);
		dup(f2);
		}
			char *newexec;
			making_exec(parsed[0],&newexec);
			//	printf("befor function %s and agter function %s",parsed[0],newexec);
				execvp(newexec,parsed);
		
		printf("There is a error\n");
		exit(0);
       
	     
      }
    else
      {
	//Parent
		waitpid(pid1,NULL,0);
	    
	
	     
		}
    
    return;
}
	
	

void my_cd(char **parsed)
{
 
    if(parsed[2]!=NULL)
	printf("too many arguments\n");
    else
      

      {
	int  a=chdir(parsed[1]);

    
	if(a!=0)
	  printf("No such directory exist\n");

	}
}

void my_mkdir(char **parsed)
{
  if(parsed[2]!=NULL)
	printf("too many arguments\n");
    else
      {
	int a=mkdir(parsed[1], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if(a!=0)
	  printf("Error\n");

      }
}

void my_rmdir(char **parsed)
  {
  if(parsed[2]!=NULL)
	printf("too many arguments\n");
    else
      {
	int a=rmdir(parsed[1]);
		if(a!=0)
	  printf("Error\n");

      }
}
  


void take_input(char *inp)
{
 
  long size =10;
  char *string;
  string = (char *) malloc (size);
  getline(&string,&size,stdin)  ;

  strcpy(inp,string);
   free(string);
   
  return ;
}

int bulletincmd( char **parsed)
{
  char *mycommands[5];
  mycommands[0]="cd";
  mycommands[1]="mkdir";
  mycommands[2]="rmdir";
  mycommands[3]="pwd";
  mycommands[4]="exit";
  //  printf("piyush");
  int i=0;
  for(;i<5;i++)
    {
      if(strcmp(parsed[0],mycommands[i])==0)
	break;
  }
  switch(i)
    {
    case 0 : my_cd (parsed);
	    return 1;
	    break;
	    
    case 1 : my_mkdir(parsed);
	    return 1;
	    break;
	    
    case 2 : my_rmdir(parsed);
	    return 1;
	    break;
	    
    case 3 : pwd();
	    return 1;
	    break;
	    
    case 4 : var=0;
	    return 1;
	    break;
    default : break;
      
     
    }
  return 0;
      
    
  
}
	
void myfunc2(char * cmd ,char **output1,char **newcmd1 ){
int len=strlen(cmd);
int lt=-1;
int mt=-1;

char *output=malloc(sizeof(char)*1000);
char *newcmd=malloc(sizeof(char)*1000);

*output1=output;
*newcmd1=newcmd;
for(int i=0;i<len;i++){
if(cmd[i]=='<')
lt=i;
if(cmd[i]=='>')
mt=i;
}

if(mt!=-1){
for(int i=0;i<mt;i++)
newcmd[i]=cmd[i];

mt++;
int index1=mt;

while(cmd[index1]==' ')
{
	index1++;
}
int k=0;
while(1)
{
	if(cmd[index1]=='<' || cmd[index1]=='|' || cmd[index1] ==' ' || cmd[index1]=='\n' || index1>=len)
	break;
	output[k]=cmd[index1];
	index1++;
	k++;
	
}
output[k]='\0';
mt--;
	while(index1<len && cmd[index1]!='\n')
	{
		newcmd[mt]=cmd[index1];
		mt++;
		index1++;
	}


	
}
 else
   {
strcpy(newcmd,cmd);
   }
	return;
	
}



void myfunc(char * cmd ,char **input1,char **newcmd1 ){
int len=strlen(cmd);
int lt=-1;
int mt=-1;
char *input=malloc(sizeof(char)*1000);

char *newcmd=malloc(sizeof(char)*1000);
*input1=input;

*newcmd1=newcmd;
for(int i=0;i<len;i++){
if(cmd[i]=='<')
lt=i;
if(cmd[i]=='>')
mt=i;
}
if(lt!=-1){
for(int i=0;i<lt;i++)
{
	newcmd[i]=cmd[i];
}
lt++;
int index1=lt;
while(cmd[index1]==' ')
{
	index1++;
}
int k=0;
while(1)
{
	if(cmd[index1]=='>' || cmd[index1]=='|' || cmd[index1] ==' ' || cmd[index1]=='\n' || index1>=len)
	break;
	input[k]=cmd[index1];
	index1++;
	k++;
	
}
input[k]='\0';

lt--;
	while(index1<len && cmd[index1]!='\n')
	{
		newcmd[lt]=cmd[index1];
		lt++;
		index1++;
	}

	
}
else
{
strcpy(newcmd,cmd);
}


	return;
	
}


void removing(char *cmd,char **newcmd1)
{
  char *newcmd=malloc(sizeof(char)*1000);
  *newcmd1=newcmd;
  int len=strlen(cmd);
  
  for(int i=0;i<len;i++)
    {
      if(cmd[i]!='\n')
      newcmd[i]=cmd[i];
      
	
    }
  return;
}


int main()
{

  
  
  while(var)
    {
     
      if(var==0)
	break;
      starting_line();
      //	printf("gupta");
      char cmd[max1];
	do{
	take_input(cmd);
      }while(strlen(cmd)==0);

	//	printf("gupta1");
	if(strlen(cmd)==1)
	  continue;
	char *pipeparsed[max1];
	
	
		char *input;
		char *output;
		char *newcmd1;
		char *newcmd2;
		char *newcmd3;
	
		//	printf("gupt2");
	char cmd2[max1];
	char cmd3[max1];
	removing(cmd,&newcmd1);
	myfunc(newcmd1,&input,&newcmd2);
	myfunc2(newcmd2,&output,&newcmd3);
        

		int num_pipe=pipe_split_new(newcmd3);
		
			if(num_pipe==0)
	{
		char *parsed1[max1];
		parse_space(newcmd2,parsed1);
        	int a=bulletincmd(parsed1);
	
		if(a==0)
		{
		  
		 	without_piped(parsed1,input,output);
		
		}
	}
		else
	{
	  
	  piped_new(newcmd3,input,output,num_pipe);
		}
	
	
      
   }
 
  

  return 0;
	
   
}
 
