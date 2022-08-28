#ifndef __XERROR_H__
#define __XERROR_H__

#include <X11/Xlib.h>

void x_register_xerror_handler();
void x_attach_display_error(Display *Display);

Window x_get_event_window(XEvent event);
const char * x_get_event_name(int event_type);
const char * x_get_error_name(int error_type);

int x_get_error_code(Display * display);
int x_get_error_str(Display *display, char * buffer, size_t lenght);

#endif