#ifndef __WM_H__
#define __WM_H__

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include "atoms.h"

typedef struct window_node_t
{
    void *extra;

    Window window;
    XWindowAttributes attributes;

    struct window_node_t * prev, * next;
} window_node_t;

typedef struct wm_callbacks_t
{
    void (*on_destroy_window)(window_node_t * node);
    void (*on_create_window)(window_node_t * node);
} wm_callbacks_t;

int wm_init( unsigned int modkey );
int wm_destroy( void );

int wm_set_callbacks( wm_callbacks_t callbacks );

int wm_get_screen( void );
Display * wm_get_display( void );
Window wm_get_root_window( void );

int wm_get_display_error( void );
const char wm_get_display_error_str( void );

Atom wm_get_atom( atom_id_t id );

unsigned int wm_get_window_count( void );
window_node_t * wm_get_first_window_node( void );

int wm_add_window( Window window );
int wm_close_window( Window window );
int wm_kill_window( Window window );

int wm_process_events( void );

int wm_get_window_attributes( Window window, XWindowAttributes* attributes );
int wm_focus_window( Window window );
int wm_move_window( Window window, int x, int y );

#endif