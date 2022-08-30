#ifndef __VISUAL_H__
#define __VISUAL_H__

#include "util.h"
#include "graphics.h"
#include "comm.h"

typedef enum visual_id_t
{
    VISUAL_SPEEDOMETER,

    VISUALS_END
} visual_id_t;

typedef void (*visual_task_cb_t)( void );

typedef struct visual_t
{
    visual_task_cb_t init;

    visual_task_cb_t draw;

    visual_task_cb_t on_sleep;
    visual_task_cb_t on_wake;

    visual_task_cb_t terminate;
} visual_t;


static const visual_t * const
visual_get_visual( visual_id_t id )
{
    extern const visual_t * const _visual_speedometer;

    switch (id)
    {
    case VISUAL_SPEEDOMETER:
        return _visual_speedometer;
        break;
    
    default:
        return NULL;
    }

    return NULL;
}

#endif