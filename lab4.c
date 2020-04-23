/* =================================================================== *
*                   Process Creation (Fork & Wait)                     *
*                                                                      *
*  Name: lab4.c                                                        *
*                                                                      *
*  Written by: Brianna Drew - March 2020                               *
*                                                                      *
*  Purpose: To use fork, execlp, wait, and waitpid to create and       *
*           control processes (three processes).                       *
*                                                                      *
*  Subroutines/Libraries Required:                                     *
*      See include statements.                                         *
* ==================================================================== */

  #include <sys/types.h>
  #include <sys/wait.h>
  #include <unistd.h>
  #include <time.h>
  #include <stdio.h>
  #include <stdlib.h>

  long GetFileSize(const char* filename);

  int main(void)
  {
     system("clear");
     int i, q, r, x, status;
     pid_t PID1, PID2, PID3, endID;
     time_t when;
     int FinishFlag = 0;
     if ((PID1 = fork()) == -1)
         {     /* Start a child process.      */
           perror("fork error");
           exit(EXIT_FAILURE);
         }
    else if (PID1 == 0)
         {            /* This is the child.          */
           printf("CHILD1: Child 1 started:%d  Parent is:%d\n",getpid(),getppid());
           if ((PID3 = fork()) < 0)
              {
                 perror("fork error");
                 exit(EXIT_FAILURE);
              }
           else if (PID3 == 0)
              {   /* This is the Grandchild */
                printf("GRANDCHILD: Grandchild started:%d  Parent is:%d\n",getpid(),getppid());
                sleep(3);
                x = execlp("/usr/bin/head", "/usr/bin/head", "-n", "20", "lab4.c", (char *)NULL);
              }
           else
              {

                while (endID != PID3)
                {  /* Test for status of GrandChild */
                    endID=waitpid(PID3, &status, WNOHANG|WUNTRACED==0);
                    if (endID == PID3)
                   {
                        /* Grandchild ended                 */
                   if (WIFEXITED(status))
                      printf("PARENT: Grandchild ended normally\n");
                   else if (WIFSIGNALED(status))
                      printf("Child ended because of an uncaught signal.n");
                   else if (WIFSTOPPED(status))
                      printf("Child process has stopped.n");
                   exit(EXIT_SUCCESS);
                  }
                /* Sleep Try loop testing for Grandchild completion */
                sleep(1);
                printf("Trying...\n");
               }
             }
        }
     else
       {  /* This is the parent.         */
          printf("Main PID:%d   Main Parent PID:%d\n",getpid(),getppid());

           /* Fork child 2*/
           if ((PID2 = fork()) < 0)
            {
               perror("fork error");
               exit(EXIT_FAILURE);
            }
          else if (PID2 == 0)
            {   /* This is Child 2 */
                printf("CHILD2: Child 2 started:%d  Parent is:%d\n",getpid(),getppid());
                getcopy();
            }


        while(i < 3 )
            {  /* Test for status of Child 1 */
             endID = waitpid(-1, &status, WNOHANG|WUNTRACED);
             if (endID == -1)
                {            /* error calling waitpid       */
                   perror("waitpid error");
                   exit(EXIT_FAILURE);
                }
           else if (endID == 0)
                {        /* child still running         */
                   time(&when);
                }
           else if (endID == PID1)
                {  /* Child 1 ended                 */
                  i++;
                  if (WIFEXITED(status))
                      printf("PARENT: Child 1 ended normally\n");
                  else if (WIFSIGNALED(status))
                      printf("PARENT: Child ended because of an uncaught signal.n");
                  else if (WIFSTOPPED(status))
                      printf("PARENT: Child process has stopped.n");
                      if (FinishFlag == 0)
                         {  /* Test whether Child 1 or Child 2 completed first */
                            FinishFlag =1;
                            printf("PARENT: Child 1 completed before Child 2\n");
                         }
                      else
                         {
                            printf("\nmd5sum file comparison\n");
                            printf("----------------------------------------------------\n");
                            execlp("/usr/bin/md5sum", "usr/bin/md5sum", "/home/COIS/3380/lab2_document.txt", "./lab4_file_copy", (char *)NULL);
                         }
                }
           else if (endID == PID2)
                {  /* Child 2 ended                 */
                   i++;
                   if (WIFEXITED(status))
                      printf("PARENT: Child 2 ended normally\n");
                   else if (WIFSIGNALED(status))
                      printf("PARENT: Child ended because of an uncaught signal.n");
                   else if (WIFSTOPPED(status))
                      printf("PARENT: Child process has stopped.n");
                      if (FinishFlag == 0)
                         {  /* Test whether Child 1 or Child 2 completed first */
                            FinishFlag =1;
                            printf("PARENT: Child 2 completed before Child 1\n");
                         }
                      else
                         {
                            printf("\nmd5sum file comparison\n");
                            printf("----------------------------------------------------\n");
                            execlp("/usr/bin/md5sum", "usr/bin/md5sum", "/home/COIS/3380/lab2_document.txt", "./lab4_file_copy", (char *)NULL);
                         }
                }
        }

     }
  }


int getcopy()
{    /* Subroutine to copy file */
 FILE *fp1, *fp2;
   char ch;
   fp1 = fopen("/home/COIS/3380/lab2_document.txt", "r");
   fp2 = fopen("lab4_file_copy", "w");

   while (1) {
      ch = fgetc(fp1);

      if (ch == EOF)
         break;
      else
         putc(ch, fp2);
   }

   printf("File copied Successfull\n\n");
   fclose(fp1);
   fclose(fp2);
   printf ("lab4_file_copy contains %ld bytes\n\n", GetFileSize("lab4_file_copy"));
exit(0);
return(0);
}


long GetFileSize(const char* filename)
{ /* Function to get size of file */
    long size;
    FILE *f;

    f = fopen(filename, "rb");
    if (f == NULL) return -1;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fclose(f);

    return size;
}
