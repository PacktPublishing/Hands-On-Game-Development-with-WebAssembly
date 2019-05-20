#include <emscripten.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    printf("main has run\n");
}

void press_up() {
    printf("PRESS UP\n");
}

void press_down() {
    printf("PRESS DOWN\n");
}

void press_left() {
    printf("PRESS LEFT\n");
}

void press_right() {
    printf("PRESS RIGHT\n");
}

void release_up() {
    printf("RELEASE UP\n");
}

void release_down() {
    printf("RELEASE DOWN\n");
}

void release_left() {
    printf("RELEASE LEFT\n");
}

void release_right() {
    printf("RELEASE RIGHT\n");
}

