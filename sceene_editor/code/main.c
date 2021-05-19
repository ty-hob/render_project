#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "view.h"


int main(){

	init();

	while(event_handler()){

		update();

		draw();

		sdl_frame_delay();

	}

	quit();

	return 0;
}

