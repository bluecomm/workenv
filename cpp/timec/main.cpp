#include <map>
#include <iostream>
#include <sstream>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
/***************************************************************************************************
 1. get time of seconds from 1970
 time_t time(time_t *calptr);

 2. calendar time into a broken-down time expressed as UTC
struct tm *gmtime(const time_t *calptr);

 3. converts the calendar time to the local time, taking into account the local time zone and
 daylight saving time flag
 struct tm *localtime(const time_t *calptr);

 4. converts it into a time_t value
 time_t mktime(struct tm *tmptr);

 5. gives the number of seconds and microseconds since the Epoch
 int gettimeofday(struct timeval *restrict tp, void *restrict tzp);
 int settimeofday(const struct timeval *tv, const struct timezone *tz);

 6. gen time like Tue Feb 10 18:27:38 2004/n/0, as 'date' cmd in shell
 char *asctime(const struct tm *tmptr);
 char *ctime(const time_t *calptr);

 7. format date and time, see: https://www.man7.org/linux/man-pages/man3/strftime.3.html
 size_t strftime(char *s, size_t max, const char *format, const struct tm *tm);

***************************************************************************************************/
// struct timeval{
//     time_t tv_sec;        /*** second ***/
//     susecond_t tv_usec;    /*** microsecond ***/
// }
// 
// struct tm {        /* a broken-down time */
//     int  tm_sec;     /* seconds after the minute: [0 - 60] */
//     int  tm_min;     /* minutes after the hour: [0 - 59] */
//     int  tm_hour;    /* hours after midnight: [0 - 23] */
//     int  tm_mday;    /* day of the month: [1 - 31] */
//     int  tm_mon;     /* months since January: [0 - 11] */
//     int  tm_year;    /* years since 1900 */
//     int  tm_wday;    /* days since Sunday: [0 - 6] */
//     int  tm_yday;    /* days since January 1: [0 - 365] */
//     int  tm_isdst;   /* daylight saving time flag: <0, 0, >0 */
// 
//     long int tm_gmtoff; /* Seconds east of UTC. */
//     const char *tm_zone; /* Timezone abbreviation. */
// };  
// *************************************************************************************************

int main () {

    time_t tt = time(NULL);
    std::cout << "time: " << tt << std::endl;

    struct timeval tv;    
    gettimeofday(&tv,NULL);
    std::cout << "gettimeofday: " << tv.tv_sec * 1000000 + tv.tv_usec << std::endl;

    struct tm *tmtm = localtime(&tt);
    std::cout << "localtime: " << tmtm->tm_year + 1900 << "-";
    std::cout << tmtm->tm_mon + 1 << "-";
    std::cout << tmtm->tm_mday << " ";
    std::cout << tmtm->tm_hour << ":";
    std::cout << tmtm->tm_min << ":";
    std::cout << tmtm->tm_sec << std::endl;

    char *readable_time = ctime(&tt); // like strftime with fmt "%a %b %e %H:%M:%S %Y"
    std::string rt(readable_time);
    rt.erase(rt.end() - 1);
    std::cout << "ctime: " << rt << std::endl;

    char fmt_time[64] = {0};
    size_t ret_size = strftime(fmt_time, 63, "%Y-%m-%d %H:%M:%S %a", tmtm);
    std::cout << "strftime(" << ret_size << "): " << fmt_time << std::endl;

    char fmt_time1[64] = {0};
    ret_size = strftime(fmt_time1, 63, "%a %b %e %H:%M:%S %Y", tmtm); // like ctime
    std::cout << "strftime(" << ret_size << "): " << fmt_time1 << std::endl;

    return 0;
}
