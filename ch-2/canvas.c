#include <emscripten.h>
#include <stdlib.h>
#include <stdio.h>
    
int ship_x = 0;
int ship_y = 0;

void MoveShip() {
    ship_x += 2;
    ship_y++;

    if( ship_x >= 800 ) {
        ship_x = -128;
    }

    if( ship_y >= 600 ) {
        ship_y = -128;
    }

    EM_ASM( ShipPosition($0, $1), ship_x, ship_y );
}


int main() {
    printf("Begin main\n");
    emscripten_set_main_loop(MoveShip, 0, 0);
    return 1;

}
