#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
        char world[] = "world";
        printf("Hello %s\n", world);
        world[1] = '\0';
        printf("Hello %s your string is %lu bytes long!\n",
               world, strlen(world));
        world[3] = 'm';
        printf("Hello %s your string is %lu bytes long!\n",
               world, strlen(world));
        world[1] = 'o';
        world[4] = '\0';
        printf("Hello %s your string is %lu bytes long!\n",
               world, strlen(world));
        world[4] = 'y';
        world[5] = '\0';
        printf("Hello %s your string is %lu bytes long!\n",
               world, strlen(world));
}