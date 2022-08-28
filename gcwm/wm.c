#include "wm.h"

#include "xerror.h"
#include "mema.h"
#include "util.h"

#ifndef WM_NAME
#define WM_NAME                                                 "evwm"
#endif

#define XERROR_MSG_LEN                                          255

#ifndef XDISPLAY_NAME
#define XDISPLAY_NAME                                           ":4.0"
#endif

typedef struct active_window_t
{
    bool defined;

    XEvent event;

    Window window;
    XWindowAttributes attributes;
} active_window_t;

typedef struct wm_t
{
    int screen;
    Display* display;

    Window root_window, check_window;
    XWindowAttributes root_window_attributes;

    uint modkey;
    Atom atoms[atom_no_id];

    window_node_t * window_root_node;
    active_window_t active_window;

    wm_callbacks_t callbacks;
} wm_t;

static wm_t wm;

static int
wm_display_error( void )
{
    return x_get_error_code(wm.display);
}

static const char*
wm_display_error_str( void )
{
    static char m[XERROR_MSG_LEN] = {0};

    x_get_error_str(wm.display, m, arrlen(m));
    return m;
}

static inline bool
has_active_window( void )
{
    return wm.active_window.defined;
}

static int
set_active_window( Window window, XEvent event )
{
    active_window_t active = {0};

    if(window == wm.root_window)
        return 1;
    
    active.defined = true;
    active.window = window;
    active.event = event;
    wm_get_window_attributes(window, &active.attributes);

    wm.active_window = active;

    return 0;
}

static inline void
clear_active_window( void )
{
    clear(&wm.active_window);
}

static void
window_grab_buttons(Window window)
{
    int err = 0;

    err = XGrabButton(
        wm.display,
        AnyButton,
        AnyModifier,
        window,
        True,
        ButtonPressMask|ButtonReleaseMask|ButtonMotionMask,
        GrabModeAsync,
        GrabModeAsync,
        None,
        None
    );

    if(err == BadCursor || err == BadValue || err == BadWindow)
        ftl("cannot grab buttons for window %lu: %s", window, x_get_error_name(err));
}

static int
count_windows( void )
{
    int count = 0;
    window_node_t * wn = NULL;

    wn = wm.window_root_node;
    for(; wn && wn->next != NULL; wn = wn->next)
        count++;

    return count;
}

static void
update_client_list( void )
{
    lmema_init();

    int i = 0, client_count = 0;
    Window *client_list = NULL;
    window_node_t * wn = NULL;
    
    client_count = count_windows();
    client_list = lcalloc(client_count, sizeof(Window));

    wn = wm.window_root_node;
    for(; wn && wn->next != NULL; wn = wn->next)
        client_list[i++] = wn->window;

    XChangeProperty(wm.display, wm.root_window, 
        wm.atoms[net_client_list], XA_WINDOW, 32, PropModeReplace, 
        (unsigned char*) client_list, client_count);

    lnreturn;
}

static window_node_t *
find_window(Window window)
{
    window_node_t * wn = NULL;

    wn = wm.window_root_node;
    for(; wn != NULL; wn = wn->next)
        if(wn->window == window) break;
    
    return wn;
}

static struct window_node_t *
add_window(Window window)
{
    window_node_t * wn = NULL, * nw = NULL;

    wn = find_window(window);
    if(wn)
    {
        dbg("window (id: %lu) already on list", window);
        return wn;
    }

    wn = wm.window_root_node;
    for(; wn && wn->next != NULL; wn = wn->next);

    nw = gcalloc(1, sizeof(window_node_t));

    // If you are adding the root window
    if(wm.window_root_node == NULL)
    {
        wm.window_root_node = nw;

        XSelectInput(wm.display, window, SubstructureNotifyMask | 
            PointerMotionMask | ButtonMotionMask | ButtonPressMask | 
            ButtonReleaseMask);
    }

    // If you are not
    else
    {
        wn->next = nw;

        XSelectInput(wm.display, window, FocusChangeMask);
        window_grab_buttons(window);
    }

    nw->prev = wn;
    nw->window = window;

    update_client_list();

    dbg("window (id: %lu) added to list", window);

    if(wm.callbacks.on_create_window)
        wm.callbacks.on_create_window(nw);

    return nw;
}

static void
remove_window(Window window)
{
    window_node_t * wn = NULL;

    wn = find_window(window);
    if(wn == NULL)
    {
        dbg("window (id: %lu) is not on list", window);
        return;
    }

    if(wn->prev)
        wn->prev->next = wn->next;
    if(wn->next)
        wn->next->prev = wn->prev;
    
    if(wm.window_root_node == wn)
        wm.window_root_node = wn->next;

    update_client_list();

    dbg("window (id: %lu) removed from list", window);

    if(wm.callbacks.on_destroy_window)
        wm.callbacks.on_destroy_window(wn);

    gfree(wn);
}

int
wm_add_window( Window window )
{
    return add_window(window) != NULL;
}

int
wm_close_window( Window window )
{
    XEvent event = {0};

    if(find_window(window) == NULL)
        return 1;

    event.xclient.type          = ClientMessage;
    event.xclient.window        = window;
    event.xclient.message_type  = wm.atoms[wm_protocols];
    event.xclient.format        = 32;
    event.xclient.data.l[0]     = wm.atoms[wm_delete_window];
    event.xclient.data.l[1]     = CurrentTime;

    XSendEvent(wm.display, window, 0, NoEventMask, &event);
    return 0;
}

unsigned int 
wm_get_window_count( void )
{
    return count_windows();
}

window_node_t * 
wm_get_first_window_node( void )
{
    if(wm.window_root_node)
        return wm.window_root_node->next;
    
    return NULL;
}

int
wm_get_screen( void )
{
    return wm.screen;
}

Display *
wm_get_display( void )
{
    return wm.display;
}

Window 
wm_get_root_window( void )
{
    return wm.root_window;
}

Atom 
wm_get_atom( atom_id_t id )
{
    return id < atom_no_id ? wm.atoms[id] : 
        wm.atoms[arrlen(wm.atoms)-1];
}

int 
wm_kill_window( Window window )
{
    return XDestroyWindow(wm.display, window);
}

int
wm_set_callbacks( wm_callbacks_t callbacks )
{
    wm.callbacks = callbacks;
    return 0;
}

int
wm_init(unsigned int modkey)
{
    unused(wm_display_error);

    int i = 0;

    clear(&wm);

    if((wm.display = XOpenDisplay(XDISPLAY_NAME)) == NULL)
        ftl("cannot open X11 display");

    // Setup X11 error handler
    x_register_xerror_handler();
    x_attach_display_error(wm.display);

    // Get default screen and window
    wm.screen = DefaultScreen(wm.display);
    wm.root_window = DefaultRootWindow(wm.display);

    XGetWindowAttributes(wm.display, wm.root_window, 
        &wm.root_window_attributes);

    // Add root window to the window list
    add_window(wm.root_window);

    wm.modkey = modkey;

    // Setup Atoms
    for(i = 0; i < atom_no_id; i++)
    {
        if(i == net_start)
            continue;
        
        wm.atoms[i] = XInternAtom( wm.display, atom_to_str(i), false);
    }
    
    // Be nice with EWMH:
    // https://specifications.freedesktop.org/wm-spec/1.3/ar01s03.html
    // Taken from dwm.c

    // Create empty, tiny window to apply the wm atoms spec into
    wm.check_window = XCreateSimpleWindow(
        wm.display, 
        wm.root_window,
        0, 0, 1, 1, 0, 0, 0
    );

    // Do the _NET_SUPPORTING_WM_CHECK thing. I'm not gonna lie, I haven't done
    // much research on this, so I should get back to this later
	XChangeProperty(wm.display, wm.check_window, 
        wm.atoms[net_supporting_wm_check], XA_WINDOW, 32, 
        PropModeReplace, (unsigned char *) &wm.check_window, 1);
	XChangeProperty(wm.display, wm.root_window, 
        wm.atoms[net_supporting_wm_check], XA_WINDOW, 32, 
        PropModeReplace, (unsigned char *) &wm.check_window, 1);

	XChangeProperty(wm.display, wm.check_window, 
        wm.atoms[net_wm_name], wm.atoms[utf8_string], 8, 
        PropModeReplace, (unsigned char *) WM_NAME, arrlen(WM_NAME));

    // Specify _NET_SUPPORTED
    XChangeProperty(wm.display, wm.root_window, 
        wm.atoms[net_wm_supported], XA_ATOM, 32, PropModeReplace,
        (unsigned char *)&wm.atoms[net_start + 1], atom_no_id - net_start -1);
    
    return 0;
}

int
wm_destroy( void )
{
    window_node_t *node = NULL, *next_node = NULL;

    node = wm.window_root_node;
    while(node)
    {
        next_node = node->next;
        gfree(node);

        node = next_node;
    }


    if(wm.display)
        return XCloseDisplay(wm.display);

    clear(&wm);
    return 0;
}

int 
wm_focus_window( Window window )
{
    if(!find_window(window))
    {
        add_window(window);
    }

	XSetInputFocus(wm.display, window, RevertToParent, CurrentTime);
	XMapRaised(wm.display, window);

    dbg("focusing on window %ld", window);
    return 0;
}

int
wm_move_window( Window window, int x, int y )
{
    int err = 0;

    if(window == wm.root_window)
        return 0;

    if(!find_window(window))
        add_window(window);
    
    err = XMoveWindow(wm.display, window, x, y);
    return err;
}

static int
button_press_handler(Window window, XEvent xevent)
{
    XButtonEvent *event = NULL;

    event = &xevent.xbutton;

    if(xevent.type == ButtonPress)
    {
        if(event->state & wm.modkey)
        {
            set_active_window(window, xevent);
            if(window != wm.root_window)
                wm_focus_window(window);

            switch (event->button)
            {
            case Button1:
                dbg("moving window (id: %lu)", event->window);
                break;
            case Button3:
                dbg("mesizing window (id: %lu)", event->window);
                break;
            
            default:
                break;
            }
        }
    }
    else
        clear_active_window();

    return 1;
}

int
wm_get_window_attributes( Window window, XWindowAttributes* attributes )
{
    return XGetWindowAttributes(wm.display, window, attributes);
}

int
wm_process_events()
{
    int ret = 0;
    window_node_t * node = NULL;
    XEvent event = {0};
    Window window = 0;

    static XEvent previous_event = {0};

    if(XPending(wm.display) < 1)
        return 0;

    ret = XNextEvent(wm.display, &event);
    if(ret != Success)
    {
        err("cannot process event: %s", wm_display_error_str());
        return 1;
    }

    if(previous_event.type != event.type && 
        previous_event.xany.window == event.xany.window)
    {
        dbg("received \"%s\" on window (id: %ld)", x_get_event_name(event.type), event.xany.window);
        previous_event = event;
    }

    window = x_get_event_window(event);
    if(window == wm.root_window)
        return 0;

    if(window)
        node = find_window(window);

    switch(event.type)
    {
        case CreateNotify:
        {            
            if(!window)
                break;

            add_window(window);
            wm_focus_window(window);

            dbg("received create event for window (id: %lu)", window);            
            break;
        }

        case DestroyNotify:
        {
            if(!window)
                break;

            remove_window(window);

            dbg("received destroy event for window (id: %lu)", window);
            break;
        }

        case ButtonPress:
        case ButtonRelease:
        {
            window_node_t * node = NULL;

            if(!(node = find_window(event.xbutton.window)))
            {
                dbg("adding unregistered window (id: %lu) to list",
                    event.xbutton.window);
                add_window(event.xbutton.window);
            }

            if(button_press_handler(node->window, event))
                XAllowEvents(wm.display, ReplayPointer, CurrentTime);
            else
                XAllowEvents(wm.display, SyncPointer, CurrentTime);
        }

        case MotionNotify:
        {
            if(wm.active_window.window != window)
                break;
            
            wm_move_window(window, 
                event.xmotion.x_root - wm.active_window.event.xbutton.x, 
                event.xmotion.y_root- wm.active_window.event.xbutton.y);
            XGetWindowAttributes(wm.display, window, &node->attributes);
        }

        case ConfigureNotify:
        case MapNotify:
        case UnmapNotify:
        {
            node = find_window(window);

            XGetWindowAttributes(wm.display, window, &node->attributes);
        }

        default:
            break;
    }

    return 0;
}