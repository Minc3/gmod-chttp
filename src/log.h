#include "tier0/dbg.h"

#define DEV(fmt, ...)        DevMsg("[chttp] " fmt "\n", ##__VA_ARGS__)
#define MSG(fmt, ...)        Msg("[chttp] " fmt "\n", ##__VA_ARGS__)
#define WARN(fmt, ...)       Warning("[chttp] " fmt "\n", ##__VA_ARGS__)


#ifdef NDEBUG

#define DBGDEV(fmt, ...)
#define DBGMSG(fmt, ...)
#define DBGWARN(fmt, ...)

#else

#define DBGDEV(fmt, ...)     DEV(fmt, ##__VA_ARGS__)
#define DBGMSG(fmt, ...)     MSG(fmt, ##__VA_ARGS__)
#define DBGWARN(fmt, ...)    WARN(fmt, ##__VA_ARGS__)

#endif