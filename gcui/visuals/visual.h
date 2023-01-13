#ifndef __VISUAL_H__
#define __VISUAL_H__

#include "util.h"
#include "graphics.h"

#define VISUAL_SOCKET_BASE                              "/tmp/"

#define VISUAL_DEFINE_STATIC(_name, _visual)            const visual_t * const _name = &_visual;
#define VISUAL_IMPORT_STATIC(_name)                     extern const visual_t * const _name;

typedef enum visual_id_t
{
    VISUAL_SPEEDOMETER,
    VISUAL_BATTERY,
    VISUAL_TIME,
    VISUAL_TEMPERATURE,
    VISUAL_BLINKERS,

    VISUALS_END
} visual_id_t;

typedef void (*visual_task_cb_t)( void );
typedef bool (*visual_draw_cb_t)( bool );

typedef struct visual_t
{
    visual_task_cb_t init;

    visual_draw_cb_t draw;

    visual_task_cb_t on_sleep;
    visual_task_cb_t on_wake;

    visual_task_cb_t terminate;
} visual_t;


static const visual_t * const
visual_get_visual( visual_id_t id )
{
    VISUAL_IMPORT_STATIC(_visual_speedometer);
    VISUAL_IMPORT_STATIC(_visual_battery);
    VISUAL_IMPORT_STATIC(_visual_time);
    VISUAL_IMPORT_STATIC(_visual_temperature);
    VISUAL_IMPORT_STATIC(_visual_blinkers);

    switch (id)
    {
    case VISUAL_SPEEDOMETER:
        return _visual_speedometer;
    case VISUAL_BATTERY:
        return _visual_battery;
    case VISUAL_TIME:
        return _visual_time;
    case VISUAL_TEMPERATURE:
        return _visual_temperature;
    case VISUAL_BLINKERS:
        return _visual_blinkers;
    
    default:
        return NULL;
    }

    return NULL;
}

#endif