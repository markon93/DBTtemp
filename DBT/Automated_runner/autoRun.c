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
  char dirName[1024];
  strcpy(filename, "");
  strcpy(dirName, "");

  char input;
  while (1){

    // Display help text
    printf("\'r\' - Run tracker\n");
    printf("\'s\' - Switch all parameters\n");
    printf("\'w\' - Switch weight\n");
    printf("\'i\' - Switch initials\n");
    printf("\'f\' - Switch number of frames\n");
    printf("\'d\' - Switch starting distance\n");
    printf("\'m\' - Switch number of markers\n");
    printf("\'e\' - Switch exercise\n");
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
    else if(input == 'i'){
      getInfo("initials", p -> initials);
    }

    // Ge new load
    else if(input == 'w'){
      getInfo("weight", p -> weight);
    }

    // Ge new load
    else if(input == 'f'){
      getInfo("number of frames", p -> nFrames);
    }

    // Ge new load
    else if(input == 'm'){
      getInfo("number of markers", p -> nReflexes);
    }

    // Ge new load
    else if(input == 'd'){
      getInfo("distance", p -> distance);
    }

    // Ge new load
    else if(input == 'e'){
      getInfo("exercise", p -> exercise);
    }

    // Run tracking program
    else if(input == 'r'){
      int pid = fork();

      if(pid < 0){
        perror("fork");
      }

      else if(pid == 0){

        // Generate directory and filename
        strcat(filename, p->initials);
        strcat(filename, "_");
        strcat(filename, p -> exercise);
        strcat(filename, "_");

        strcat(dirName, filename);
        strcat(dirName, date);
        strcat(dirName, "/");

        strcat(filename, p -> weight);
        strcat(filename, "_");
        strcat(filename, date);

        strcat(dirName, filename);

        char fileArg[1024];
        strcpy(fileArg,"");
        strcat(fileArg,"filename=");
        strcat(fileArg,dirName);

        char frameArg[1024];
        strcpy(frameArg,"");
        strcat(frameArg,"nFrames=");
        strcat(frameArg,p->nFrames);

        char refArg[1024];
        strcpy(refArg,"");
        strcat(refArg,"nMarkers=");
        strcat(refArg, p->nReflexes);

        char distArg[1024];
        strcpy(distArg,"");
        strcat(distArg,"StartDist=");
        strcat(distArg, p-> distance);

        // Arguments for tracking program
        char* args[] = {"./track", "test", frameArg,
        distArg, refArg};

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
