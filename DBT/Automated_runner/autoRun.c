#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void flush(){
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

void getInfo(char* param, char* info){
  printf("Enter %s: ", param);
  fgets(info, 100, stdin);
  char* pos;
  if((pos = strchr(info, '\n')) != NULL){
    *pos = '\0';
  }
}

typedef struct params{
  char* initials;
  char* exercise;
  char* weight;
  char* nReflexes;
  char* nFrames;
  char* distance;
} params;

void getAllParams(params* p){
  char* initials = malloc(100*sizeof(char*));
  getInfo("initials", initials);
  p -> initials = initials;

  char* exercise = malloc(100*sizeof(char*));
  getInfo("exercise", exercise);
  p -> exercise = exercise;

  char* weight = malloc(100*sizeof(char*));
  getInfo("weight", weight);
  p -> weight = weight;

  char* nReflexes = malloc(100*sizeof(char*));
  getInfo("number of reflexes", nReflexes);
  p -> nReflexes = nReflexes;

  char* nFrames = malloc(100*sizeof(char*));
  getInfo("number of frames", nFrames);
  p -> nFrames = nFrames;

  char* distance = malloc(100*sizeof(char*));
  getInfo("distance", distance);
  p -> distance = distance;
}

void freeAllParams(params* p){
  if(p -> nReflexes != NULL)
    free(p -> nReflexes);
  if(p -> distance != NULL)
    free(p -> distance);
  if(p -> initials != NULL)
    free(p -> initials);
  if(p -> nFrames != NULL)
    free(p -> nFrames);
  if(p -> weight != NULL)
    free(p -> weight);
  if(p -> exercise != NULL)
    free(p -> exercise);
}

int main (int argc, char* argv[]){

  params* p = malloc(sizeof(params));
  getAllParams(p);

  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  printf("%d-%d-%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

  char year[5];
  sprintf(year, "%d", tm.tm_year + 1900);

  char month[3];
  sprintf(month, "%d", tm.tm_mon + 1);

  char day[5];
  sprintf(day, "%d", tm.tm_mday);

  char date[13];
  strcpy(date, "");
  strcat(date, year);
  strcat(date, month);
  strcat(date, day);


  char filename[1024];
  strcpy(filename, "");

  char input;
  while (1){

    // Display help text
    printf("\'r\' - Run tracker\n");
    printf("\'s\' - Switch parameters\n");
    printf("\'w\' - Switch weight\n");
    printf("\'q\' - Quit\n");
    printf("Enter command: ");

    // Get command
    input = getchar();
    flush();

    if(input == 'q'){
      break;
    }

    // Get new parameters
    else if (input == 's'){
      freeAllParams(p);
      getAllParams(p);
    }

    // Ge new load
    else if(input == 'w'){
      getInfo("weight", p -> weight);
    }

    // Run tracking program
    else if(input == 'r'){
      int pid = fork();

      if(pid < 0){
        perror("fork");
      }

      else if(pid == 0){

        // Generate filename

        strcat(filename, p->initials);
        strcat(filename, "_");
        strcat(filename, p -> exercise);
        strcat(filename, "_");
        strcat(filename, p -> weight);
        strcat(filename, "_");
        strcat(filename, date);

        // Arguments for tracking program
        char* args[] = {"./track", filename, p -> nFrames,
        p -> distance, p-> nReflexes, NULL};

        execvp(args[0], args);

      }
      else{
        waitpid(pid, NULL,0);
      }
    }
  }
  freeAllParams(p);
  return 0;
}
