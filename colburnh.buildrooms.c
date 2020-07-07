#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>

struct Room {
    char *name;
    char *type;
    char *connection;
    struct Room *next;
};

void createDir(){

    char dirName[] = "colburnh.rooms.";

    /* generate random number */
    srand(time(0));
    int num = (rand() % 100000);

    /* allocate space for directory name */
    int length = snprintf(NULL, 0, "%d", num);
    char* str = malloc(length + 1);
    snprintf(str, length + 1, "%d", num);

    char src[50], dest[50];

    /* copy & concatenate base name w/ random int */
    strcpy(src, str);
    strcpy(dest, dirName);
    strcat(dest, src);

    str = dest;

    /* create directory with new directory name */
    mkdir(str, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    return;
}

char *getFilePath(char *directoryName, char *fileName){
    /* adapted from class material */
    /* copy parts of path and concatenate them in correct order */
    char *filePath = malloc(strlen(directoryName) + strlen(fileName) + 2);
    memset(filePath, '\0', strlen(directoryName) + strlen(fileName) + 2);
    strcpy(filePath, directoryName);
    strcat(filePath, "/");
    strcat(filePath, fileName);
    return filePath;
}

int getRoomNum(){
    /* random integer generating algorithm is adapted from the Knuth algorithm */
    int usedNums[10], roomNum;
    srand(time(0));
    const int M = 7;
    const int N = 10;

    int in, im;
    im = 0;

    /* integer is generated based on probability */
    for (int i = 0; in < N && im < M; in++) {
        int rn = N - in;
        int rm = M - im;
        if (rand() % rn < rm)
            usedNums[im++] = in;
    }

    return *usedNums;
}

char *findLatestDir() {
    /* adapted from class material */
    /* set up */
    struct stat dirStat;
    char directoryName[256];
    char *latestDirName;
    char *path = ".";

    /* open directory */
    DIR* currDir = opendir(path);
    struct dirent *aDir;
    time_t lastModifTime;
    int i = 0;
    char *prefix = "colburnh.rooms.";

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

    /* create name of most recent directory */
    latestDirName = malloc(sizeof(char) * (strlen(directoryName) + 1));
    strcpy(latestDirName, directoryName);
    return latestDirName;
}

int makeRoom(int size, int usedNums[]){
    char names[10][9] = {"coffee", "tea", "water", "beer", "wine", "cocktail", "milk", "smoothie", "soda", "seltzer"};
    int roomNum;
    /* check if room num has already been used */
    for (int i = 0; i < 7; i++) {

        /* create room file */
        char *roomName = &names[i][0];
        struct Room currentRoom = {.name = roomName};
        printf("room name: %s\n", currentRoom.name);

        if (size == 10){
            currentRoom.type = "START_ROOM";
        } else if (size == 9){
            currentRoom.type = "END_ROOM";
        } else {
            currentRoom.type = "MID_ROOM";
        }

        printf("type: %s\n", currentRoom.type);
        size--;

        /* create room file */
        char *fileTail = "_room.txt";

        /* allocate space for file name */
        char *fileName = calloc(strlen(fileTail) + 9, sizeof(char));

        /* concatenate room name and fileTail */
        strcpy(fileName, currentRoom.name);
        strcat(fileName, fileTail);

        /* get most recent directory */
        char *latestDirName = findLatestDir();

        /* get file path */
        char *filePath = getFilePath(latestDirName, fileName);
        printf("filePath is %s\n", filePath);
        int fileDescriptor;

        /* create file */
        fileDescriptor = open(filePath, O_RDWR | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR);
        if (fileDescriptor == -1){
            printf("open() failed on \"%s\"\n", fileName);
            perror("Error");
            exit(1);
        }

        /* process file */

        

        // Close the file descriptor
        close(fileDescriptor);
    }
    return size;
}

int main(void) {
    /* create directory */
    createDir();

    /* choose 7 at random from 10 names */
    int size = 10;
    
    /* generate room files */
    int numList = getRoomNum();
    makeRoom(size, &numList);
    return 0;

    /* create room connections */
}