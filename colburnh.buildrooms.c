#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

struct Room {
    char *name; // do these need to be pointers?
    char *type;
    int dest;
    struct Room *next;
};
struct Room* next;

struct roomList {
    struct Room *head;
};

struct Graph {
    int rooms;
    struct roomList *connPtr;
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
    int closedir(DIR *dirp);
    free(str);

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
    free(filePath);
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

struct Graph* makeGraph(int numRooms) {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->rooms = numRooms;

    graph->connPtr = (struct roomList*)malloc(numRooms * sizeof(struct roomList));

    int i;
    for (i = 0; i < numRooms; i++) {
        graph->connPtr[i].head = NULL;
    }

    return graph;
}

int makeRoom(int size, int usedNums[]){
    char names[10][9] = {"coffee", "tea", "water", "beer", "wine", "cocktail", "milk", "smoothie", "soda", "seltzer"};
    int roomNum;
    char *roomPtr;

    for (int i = 0; i < 7; i++) {

        /* set up pointers */
        char *roomName = &names[i][0];
        struct Room *roomList[i], currentRoom = {.name = roomName};
        roomList[i] = &currentRoom;

        /* assign room type */
        if (size == 10){
            currentRoom.type = "START_ROOM";
        } else if (size == 9){
            currentRoom.type = "END_ROOM";
        } else {
            currentRoom.type = "MID_ROOM";
        }

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
        int fileDescriptor;

        /* create file */
        fileDescriptor = open(filePath, O_RDWR | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR);
        if (fileDescriptor == -1){
            printf("open() failed on \"%s\"\n", fileName);
            perror("Error");
            exit(1);
        }

        /* process file */
        /* allocate space for room name line */
        char roomNameLine[] = "ROOM NAME: ";
        char *nameLine = calloc(strlen(roomName) + 9, sizeof(char));

        /* concatenate roomNameLine and name of room */
        strcpy(nameLine, roomNameLine);
        strcat(nameLine, currentRoom.name);
        
        /* write to file */
        write(fileDescriptor, nameLine, strlen(nameLine) + 1);

        // Close the file descriptor
        close(fileDescriptor);
    }
    return size;
}

int getRandomRoom() {
    return (rand() % 6);
}

struct Room* makeNewRoom(int dest) {
    struct Room* newRoom = (struct Room*)malloc(sizeof(struct Room));
    newRoom->dest = dest;
    newRoom->next = NULL;

    return newRoom;
}

// Returns true if Rooms x and y are the same Room, false otherwise
bool isSameRoom(int roomX, int roomY) {
    if (roomX == roomY)
        return true;
    else
        return false;
}

// Connects Rooms x and y together, does not check if this connection is valid
void connectRoom(struct Graph* graph, int roomX, int roomY) {
    // Add an edge from src (X) to dest (Y).  A new node is added to the adjacency
    // list of src.  The node is added at the begining
    struct Room* newRoom = makeNewRoom(roomY);
    newRoom->next = graph->connPtr[roomX].head;
    graph->connPtr[roomX].head = newRoom;
 
    // Since graph is undirected, add an edge from dest to src also
    newRoom = makeNewRoom(roomX);
    newRoom->next = graph->connPtr[roomY].head;
    graph->connPtr[roomY].head = newRoom;
    printf("success\n");
}

// Returns true if a connection from Room x to Room y already exists, false otherwise
bool connectionAlreadyExists(struct Graph* graph, int roomX, int roomY) {
    struct Room* pCrawl = graph->connPtr[roomX].head;
    while (pCrawl) {
        if (pCrawl->dest == roomY) {
            return true;
        }
        pCrawl = pCrawl->next;
    }
    return false;
}

// Returns true if a connection can be added from Room x (< 6 outbound connections), false otherwise
bool canAddConnectionFrom(struct Graph* graph, int roomX) {
    int count = 0;
    struct Room* pCrawl = graph->connPtr[roomX].head;
    while (pCrawl) {
        count++;
        pCrawl = pCrawl->next;
    }
    if (count < 6) {
        return true;
    }
    return false;
}

// Adds a random, valid outbound connection from a Room to another Room
void addRandomConnection(struct Graph* graph) {
    int roomA;
    int roomB;

    while(true) {
        roomA = getRandomRoom();

        if (canAddConnectionFrom(graph, roomA) == true)
            break;
    } 
    do {
        roomB = getRandomRoom();

    } while(canAddConnectionFrom(graph, roomB) == false || isSameRoom(roomA, roomB) == true || connectionAlreadyExists(graph, roomA, roomB) == true); {
        connectRoom(graph, roomA, roomB);  // TODO: Add this connection to the real variables, 
        connectRoom(graph, roomB, roomA);  //  because this A and B will be destroyed when this function terminates
    }
}

// Returns true if all rooms have 3 to 6 outbound connections, false otherwise
bool isGraphFull(struct Graph* graph) {

    int v;

    /* traverse linked list to find number of elements in each list */  
    for (v = 0; v < graph->rooms; ++v) {
        int count = 0;
        struct Room* pCrawl = graph->connPtr[v].head;
        while (pCrawl) {
            pCrawl = pCrawl->next;
            count++;
            if (pCrawl->next == NULL) {
                if (count < 3) {
                printf("not full\n");
                return false;
                }
            }
    /* if any list has fewer than 3 connections, the graph is not full */
        }
    }
    return true;
}

int makeConnections(struct Graph* graph) {
    printf("here?\n");
    // Create all connections in graph
    while (isGraphFull(graph) == false)
    {
        printf("not full yet\n");
        addRandomConnection(graph);
    }

    return 0;
}

// Print the graph
void printGraph(struct Graph* graph) {
    int v;
    for (v = 0; v < graph->rooms; ++v) {
        struct Room* pCrawl = graph->connPtr[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl) {
            printf("-> %d", pCrawl->dest);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

int main(void) {
    
    int numRooms = 7;

    /* create directory */
    createDir();

    /* choose 7 at random from 10 names */
    int size = 10;
    int numList = getRoomNum();


    struct Graph *graph = makeGraph(numRooms);
    // connectRoom(graph, 0, 1);
    // connectRoom(graph, 0, 4);
    // connectRoom(graph, 1, 2);
    // connectRoom(graph, 1, 3);
    // connectRoom(graph, 1, 4);
    // connectRoom(graph, 2, 3);
    // connectRoom(graph, 3, 4);
    /* create room connections */
    //makeConnections(graph);
    printGraph(graph);

    // /* generate room files */
    // makeRoom(size, &numList);
    return 0;

}