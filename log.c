#include "log.h"

// Writes current time on HH:MM:SS format
// on buf with a max size of len
void
get_time_str(char *buf, size_t len)
{
    time_t tt;
    struct tm * ti;

    time(&tt);
    ti = localtime(&tt);

    snprintf(buf, len, 
        "%02d:%02d:%02d", 

        ti->tm_hour,
        ti->tm_min, 
        ti->tm_sec
    );
}