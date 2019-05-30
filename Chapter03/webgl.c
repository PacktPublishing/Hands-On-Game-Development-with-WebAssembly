#include <emscripten.h>
#include <stdlib.h>
#include <stdio.h>

float ship_x = 0.0;
float ship_y = 0.0;

void MoveShip() {
    ship_x += 0.002;
    ship_y += 0.001;

    if( ship_x >= 1.16 ) {
        ship_x = -1.16;
    }

    if( ship_y >= 1.21 ) {
        ship_y = -1.21;
    }

    EM_ASM( ShipPosition($0, $1), ship_x, ship_y );
}


int main() {
    emscripten_set_main_loop(MoveShip, 0, 0);
    return 1;

}
