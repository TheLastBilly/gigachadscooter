#include "xerror.h"

#include <string.h>

#include "util.h"
#include "mema.h"

struct display_error_ev_t
{
    Display *display;
    XErrorEvent event;

    struct display_error_ev_t *next;
};
static struct display_error_ev_t *display_errors = NULL;

const static char *
XEVENT_NAMES[LASTEvent] =
{
    [KeyPress]          = "KeyPress",
    [KeyRelease]        = "KeyRelease",
    [ButtonPress]       = "ButtonPress",
    [ButtonRelease]     = "ButtonRelease",
    [MotionNotify]      = "MotionNotify",
    [EnterNotify]       = "EnterNotify",
    [LeaveNotify]       = "LeaveNotify",
    [FocusIn]           = "FocusIn",
    [FocusOut]          = "FocusOut",
    [KeymapNotify]      = "KeymapNotify",
    [Expose]            = "Expose",
    [GraphicsExpose]    = "GraphicsExpose",
    [NoExpose]          = "NoExpose",
    [VisibilityNotify]  = "VisibilityNotify",
    [CreateNotify]      = "CreateNotify",
    [DestroyNotify]     = "DestroyNotify",
    [UnmapNotify]       = "UnmapNotify",
    [MapNotify]         = "MapNotify",
    [MapRequest]        = "MapRequest",
    [ReparentNotify]    = "ReparentNotify",
    [ConfigureNotify]   = "ConfigureNotify",
    [ConfigureRequest]  = "ConfigureRequest",
    [GravityNotify]     = "GravityNotify",
    [ResizeRequest]     = "ResizeRequest",
    [CirculateNotify]   = "CirculateNotify",
    [CirculateRequest]  = "CirculateRequest",
    [PropertyNotify]    = "PropertyNotify",
    [SelectionClear]    = "SelectionClear",
    [SelectionRequest]  = "SelectionRequest",
    [SelectionNotify]   = "SelectionNotify",
    [ColormapNotify]    = "ColormapNotify",
    [ClientMessage]     = "ClientMessage",
    [MappingNotify]     = "MappingNotify",
    [GenericEvent]      = "GenericEvent"
};

const static char *
XERROR_NAME[LastExtensionError] =
{
    [Success]           = "Success",
    [BadRequest]        = "BadRequest",
    [BadValue]          = "BadValue",
    [BadWindow]         = "BadWindow",
    [BadPixmap]         = "BadPixmap",
    [BadAtom]           = "BadAtom",
    [BadCursor]         = "BadCursor",
    [BadFont]           = "BadFont",
    [BadMatch]          = "BadMatch",
    [BadDrawable]       = "BadDrawable",
    [BadAccess]         = "BadAccess",
    [BadAlloc]          = "BadAlloc",
    [BadColor]          = "BadColor",
    [BadGC]             = "BadGC",
    [BadIDChoice]       = "BadIDChoice",
    [BadName]           = "BadName",
    [BadLength]         = "BadLength",
    [BadImplementation] = "BadImplementation"
};

// Find the display error given display d. If error was not
// found, return NULL
static inline struct display_error_ev_t*
find_display_error(Display *d)
{
    struct display_error_ev_t *de = display_errors;

    while(de)
        if((void*)de->display == (void*)d)
            break;

    return de;
}

// X11 error handler. For now we just want it
// to set error_event and to attach a display to the chain if
// it's not already there
static int
x_error_handler(Display *d, XErrorEvent *e)
{
    struct display_error_ev_t *de = NULL;

    de = find_display_error(d);
    if(!de)
    {
        x_attach_display_error(d);

        // I know this is not efficient, but it's nice to double check
        de = find_display_error(d);
    }
    
    de->event = *e;
    return 0;
}

// Returns the last error code emitted for display
int 
x_get_error_code(Display * display)
{
    struct display_error_ev_t *de = NULL;

    de = find_display_error(display);
    if(!de)
        return -1;

    return de->event.error_code;
}

const char *
x_get_event_name(int event_type)
{
    if(event_type >= arrlen(XEVENT_NAMES))
        return "UNKNOWN";
    
    return XEVENT_NAMES[event_type];
}

Window
x_get_event_window( XEvent event )
{
    switch (event.type)
    {
    case KeyPress:
    case KeyRelease:
        return event.xkey.window;

    case ButtonRelease:
    case ButtonPress:
        return event.xbutton.window;

    case MotionNotify:
        return event.xmotion.window;

    case EnterNotify:
    case LeaveNotify:
        return event.xcrossing.window;
        return event.xcrossing.window;

    case FocusIn:
    case FocusOut:
        return event.xfocus.window;
        
    case KeymapNotify:
        return event.xkeymap.window;

    case Expose:
        return event.xexpose.window;

    case VisibilityNotify:
        return event.xvisibility.window;

    case CreateNotify:
        return event.xcreatewindow.window;

    case DestroyNotify:
        return event.xdestroywindow.window;

    case UnmapNotify:
        return event.xunmap.window;

    case MapNotify:
        return event.xmap.window;

    case MapRequest:
        return event.xmaprequest.window;

    case ReparentNotify:
        return event.xreparent.window;

    case ConfigureNotify:
        return event.xconfigure.window;

    case ConfigureRequest:
        return event.xconfigurerequest.window;

    case GravityNotify:
        return event.xgravity.window;

    case ResizeRequest:
        return event.xresizerequest.window;

    case CirculateNotify:
        return event.xcirculate.window;
    
    case CirculateRequest:
        return event.xcirculaterequest.window;

    case PropertyNotify:
        return event.xproperty.window;

    case SelectionClear:
        return event.xselectionclear.window;

    case ColormapNotify:
        return event.xcolormap.window;

    case ClientMessage:
        return event.xclient.window;

    case MappingNotify:
        return event.xmapping.window;
    
    default:
        return 0;
    }
}

const char *
x_get_error_name(int error_name)
{
    if(error_name >= arrlen(XERROR_NAME))
        return "UNKNOWN";
    
    return XERROR_NAME[error_name];
}

// Writes into buffer the last error received from display.
// Can only work if x_attach_display_error was called on display beforehand
int
x_get_error_str(Display *display, char * buffer, size_t lenght)
{
    const char *default_msg = "None";
    struct display_error_ev_t *de = NULL;

    if(!display)
        return -1;

    // Copy the default message into buffer
    memcpy(buffer, (void *)default_msg, min(lenght*sizeof(*buffer),
        sizeof(default_msg)));

    de = find_display_error(display);
    
    // If the display was registered, copy the error string into
    // buffer
    if(de)
    {
        XGetErrorText(
            display, 
            de->event.error_code, 
            buffer, 
            lenght*sizeof(*buffer)
        );

        return 0;
    }

    return -1;
}

// Attaches display to error event chain. You should call this on
// display before calling x_get_error_str
void
x_attach_display_error(Display *display)
{
    struct display_error_ev_t *de = display_errors, *ne = NULL;

    gmema_init();
    
    abort_if_null(display);

    if(find_display_error(display))
        return;

    ne = gcalloc(1, sizeof(*display_errors));
    ne->display = display;

    if(!de)
    {
        display_errors = ne;
        return;
    }
    else
    {
        while(de->next)
            de = de->next;
        de->next = ne;
    }

    XSynchronize(display, 0);
}

// Attaches the interal error handler to X11
// Needs to be called in order to receive any errors from X11
void
x_register_xerror_handler()
{
    XSetErrorHandler(x_error_handler);
}