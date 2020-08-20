#include "GarrysMod/Lua/Interface.h"

#include "HTTPRequest.h"
#include "ResultQueueData.h"
#include "LockableQueue.h"

// Different queues for different things
LockableQueue<HTTPRequest*>& getRequestQueue();
LockableQueue<ResultQueueData*>& getResultQueue();

bool scheduleRequest(HTTPRequest *request);

bool startThread();
