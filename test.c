#define  _GNU_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define DIR_PREFIX "students."
#define DATA_FILE "student_info.txt"

/*This programs
* Opens the current directory
* Finds the latest directory inside the current directory that has the prefix "students."
* Goes to this latest directory and proceses the file named "student_info.txt" 
* Creates a linked list with entries for each row of data found in "student_info.txt"
* When done processing the file, print all the student data stored in the linked list
*/

/* struct type for student information */
typedef struct studentType{
        char* onid;
        char* lastName;
        char* firstName;
        char* major;
        struct studentType* next;
} student;

/*
* Go to the directory specified by path. In this direcory,
* look for all the directories whose name start with prefix,
* return the name of the directory that has the latest
* modification time.
*/
char* findLatestDirectory(char* path, char* prefix){
  struct stat dirStat;
  char directoryName[256];
  char* latestDirName;

  // Open the directory
  DIR* currDir = opendir(path);
  struct dirent *aDir;
  time_t lastModifTime;
  int i = 0;

  /* The data returned by readdir() may be overwritten by subsequent calls to  readdir()  for  the  same  directory stream. So we will copy the name of the directory to the variable directoryName
  */ 
  while((aDir = readdir(currDir)) != NULL){
    // Use strncmp to check if the directory name matches the prefix
    if(strncmp(prefix, aDir->d_name, strlen(prefix)) == 0){
      stat(aDir->d_name, &dirStat);
      // Check to see if this is the directory with the latest modification time
      if(i == 0 || difftime(dirStat.st_mtime, lastModifTime) > 0){
        lastModifTime = dirStat.st_mtime;
        memset(directoryName, '\0', sizeof(directoryName));
        strcpy(directoryName, aDir->d_name);
      }
      i++;
    }
  }
  latestDirName = malloc(sizeof(char) * (strlen(directoryName) + 1));
  strcpy(latestDirName, directoryName);

  closedir(currDir);
  return latestDirName;
}


/* Create and return a string for the file path by concatenating the directory name with the file name */
char* getFilePath(char* directoryName, char* fileName){
  char* filePath = malloc(strlen(directoryName) + strlen(fileName) + 2);
  memset(filePath, '\0', strlen(directoryName) + strlen(fileName) + 2);
  strcpy(filePath, directoryName);
  strcat(filePath, "/");
  strcat(filePath, fileName);
  return filePath;
}

/* Parse the current line and create a student entry with the data */
student* createStudent(char* currLine){
  student* currStudent = malloc(sizeof(student));

  // The first token is the ONID
  char* token = strtok(currLine, " ");
  currStudent->onid = calloc(strlen(token) + 1, sizeof(char));
  strcpy(currStudent->onid, token);

  // The second token is the lastName
  token = strtok(NULL, " ");
  currStudent->lastName = calloc(strlen(token) + 1, sizeof(char));
  strcpy(currStudent->lastName, token);

  // The third token is the firstName
  token = strtok(NULL, " ");
  currStudent->firstName = calloc(strlen(token) + 1, sizeof(char));
  strcpy(currStudent->firstName, token);

  // The last token is the major
  token = strtok(NULL, "\n");
  currStudent->major = calloc(strlen(token) + 1, sizeof(char));
  strcpy(currStudent->major, token);
  
  // Set the next node to NULL in the newly created student entry
  currStudent->next = NULL;

  return currStudent;

}

/*
* Return a linked list of students by parsing data from
* each line of the specified file.
*/
student* processFile(char* filePath){
  // Open the specified file for reading only
  FILE* studentFile = fopen(filePath, "r");

  char *currLine = NULL;
  size_t len = 0;
  ssize_t nread;
  char*     token;

  // The head of the linked list
  student* head = NULL;
  // The tail of the linked list
  student* tail = NULL;

  // Read all the lines in the file
  while ((nread = getline(&currLine, &len, studentFile)) != -1) {

    // Get the student element corresponding to the current line
    student* newElem = createStudent(currLine);

    // Is this the first element in the linked list?
    if(head == NULL){
      // This is the first element in the linked link
      // Set the head and the tail to this element
      head = newElem;
      tail = newElem;
    } else{
      // This is not the first element. 
      // Add this element to the list and advance the tail
      tail->next = newElem;
      tail = newElem;
    }
  }
  free(currLine);
  return head;
}

/*
* Print the linked list of students
*/
void printStudentList(student* list){
  while(list != NULL){
    printf("%s %s %s %s\n", list->onid, list->lastName,
        list->firstName, list->major);
    list = list->next;
  }
}

int main(void){

  char* latestDir = findLatestDirectory(".", DIR_PREFIX);
  printf("latest directory is %s\n", latestDir);

  char* filePath = getFilePath(latestDir, DATA_FILE);
  printf("filePath is %s\n", filePath);

  student* list = processFile(filePath);
  printStudentList(list);
	
	return 0;
}