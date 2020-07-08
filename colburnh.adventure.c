#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>

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

int main(void) {

    /* find and open most recent set of rooms */
    char *latestDirName = findLatestDir();

    char roomName[] = "test room";
    //char nextRoom[20];
    int roomNum = 0;

    char *buffer;
    size_t bufsize = 32;
    size_t characters; //wrong

    buffer = (char *)malloc(bufsize * sizeof(char));
    if (buffer == NULL) {
        perror("Unable to allocate buffer\n");
        exit(1);
    }

    /* prompt user to choose a room */
    printf("CURRENT LOCATION: %s\n", roomName);
    printf("POSSIBLE CONNECTIONS: %s\n", roomName);
    printf("Where to? >");
    char nextRoom = getline(&buffer, &bufsize, stdin);
    //scanf("%s", nextRoom);
    printf("We're going to %s!\n", buffer);  /* delete before submitting */

    /* check validity */

        /* if valid, if end room, display winning info */
        printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
        printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", roomNum);
        printf("%s\n", nextRoom);

        /* if valid, not end room */

            /* add new room to list of visited rooms */

            /* increment step count */

            /* update current room status, prompt user */

        /* if invalid, display error message and prompt user again */
        printf("HUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n");

    return 0;
}
