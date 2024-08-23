#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "editor/view.h"

int main() {

  init();

  while (event_handler()) {

    update();

    draw();

    sdl_frame_delay();
  }

  quit();

  return 0;
}
