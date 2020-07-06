#include <stdio.h>

int main(void) {

    /* find and open most recent set of rooms */

    char roomName[] = "test room";
    char nextRoom[20];
    int roomNum = 0;

    /* prompt user to choose a room */
    printf("CURRENT LOCATION: %s\n", roomName);
    printf("POSSIBLE CONNECTIONS: %s\n", roomName);
    printf("Where to? ");
    scanf("%s", nextRoom);
    printf("We're going to %s!\n", nextRoom);  /* delete before submitting */

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
