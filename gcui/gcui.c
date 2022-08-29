#include "graphics.h"
#include "log.h"
#include "util.h"
#include "comm.h"

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#define BACKGROUND_COLOR                    0x181A18FF

int 
main(int argc, char const *argv[])
{
    comm_t comm = {0};
    comm_buffer_t buffer = {0};

    comm_init(&comm, "/tmp/test.sock");
    graphics_init();
    while(1)
    {
        comm_read(&comm, &buffer);
        graphics_set_background_color(GRAPHICS_HEX2RGBA(BACKGROUND_COLOR));
        if(buffer.len > 0)
        {
            graphics_clear();
            graphics_draw_text(GRAPHICS_FONT_MONOID_128, -1, -1, GRAPHICS_HEX2RGBA(0xffffffff), buffer.data);
            graphics_render();
        }
        usleep(100);
    }
    graphics_terminate();
    comm_destroy(&comm);
    return 0;
}
