#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define ROOM_SUFFIX "_room.txt"

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
    mkdir(str, O_RDWR | O_CREAT | O_TRUNC);

    return;
}

int getRoomNum(int size){

    srand(time(0));
    int roomNum = (rand() % size);
    printf("num in func: %d\n", roomNum);
//     /* fix this */
//     /* do {
//         int roomNum = (rand() % size);
//         printf("num in func: %d\n", roomNum);
//         used = checkList(roomNum, size);
//         printf("size in func: %d\n", size);
//     } while(used == 0); */

    return roomNum;
}

// int makeRoom(){

//     /* create room file */
//     char *fileTail = "_room.txt";
//     printf("file tail: %s\n", fileTail);

//     return 0;
// }

// int checkList(int i, int size){
//     int usedNums[10];
//     int j;
//     for (j = 0; j < size; i++)
//     {
//         if (usedNums[j] == i) {
//             return 0;  /* it was found */
//         }
//    }
//    return 1;  /* if it was not found */
// }

int main(void) {

    /* create directory */
    createDir();

    /* generate room files */

    char names[10][9] = {"coffee", "tea", "water", "beer", "wine", "cocktail", "milk", "smoothie", "soda", "seltzer"};
    int usedNums[10], current;
    int counter = 0;

    printf("num of elements in array: %d\n", counter);

    /* choose 7 at random from 10 names */
    int size = 10;
    int roomNum = getRoomNum(size);

    /* check if room num has already been used */

    for (current = 0; current < size; current++)
    {
        if (usedNums[current] == roomNum) {   /* If required element is found */
            printf("%d is present at location %d.\n", roomNum, current + 1);
            // get another number
            break;
            }
        }
        if (current == size) {
            printf("%d isn't present in the array.\n", roomNum);
            usedNums[counter] = roomNum;
        }

    printf("room num: %d\n", roomNum);
    char *roomName = &names[roomNum][0];
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

    /* create room file */
    char *fileTail = "_room.txt";

    /* allocate space for file name */
    char *fileName = calloc(strlen(fileTail) + 9, sizeof(char));

    /* concatenate room name and fileTail */
    strcpy(fileName, currentRoom.name);
    strcat(fileName, fileTail);

    printf("file name: %s\n", fileName);

    int fileDescriptor;

    fileDescriptor = open(fileName, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fileDescriptor == -1){
		printf("open() failed on \"%s\"\n", fileName);
		perror("Error");
		exit(1);
	}

    // We write a string to the file
    // char *roomInfo = currentRoom.name;
    // int howMany = write(fileDescriptor, roomInfo, strlen(roomInfo) + 1);
    // printf("wrote %d bytes to the file\n", howMany);
    
    //fwrite (&currentRoom, sizeof(struct Room), 1, fileDescriptor);
    // if (fwrite != 0) {
    //     printf("success");
    // } else
    // {
    //     printf("error");
    // }
    
    // Close the file descriptor
    //close(fileDescriptor);

    // makeRoom();
    // size--;
    // printf("size: %d\n", size);

    /* for (int i = 0; i < 7; i++){
        int roomNum = getRoomNum(size);
        printf("room num: %d\n", roomNum);
        makeRoom(i, size, &names[i]);
        printf("room name: %s\n", names[i].name);
        size--;
        printf("size: %d\n", size);

    } */

    return 0;

    /* create room connections */
}