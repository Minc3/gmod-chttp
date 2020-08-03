#include "tier0/dbg.h"

#define DEVL(l, fmt, ...)    DevMsg(l, "[chttp] " fmt "\n", ##__VA_ARGS__)
#define DEV(fmt, ...)        DEVL(1, fmt, ##__VA_ARGS__)
#define MSG(fmt, ...)        Msg("[chttp] " fmt "\n", ##__VA_ARGS__)
#define WARN(fmt, ...)       Warning("[chttp] " fmt "\n", ##__VA_ARGS__)


#ifdef NDEBUG

#define DBGDEVL(fmt, ...)
#define DBGDEV(fmt, ...)
#define DBGMSG(fmt, ...)
#define DBGWARN(fmt, ...)

#else

#define DBGDEVL(l, fmt, ...) DEVL(l, fmt, ##__VA_ARGS__)
#define DBGDEV(fmt, ...)     DEV(fmt, ##__VA_ARGS__)
#define DBGMSG(fmt, ...)     MSG(fmt, ##__VA_ARGS__)
#define DBGWARN(fmt, ...)    WARN(fmt, ##__VA_ARGS__)

#endif
