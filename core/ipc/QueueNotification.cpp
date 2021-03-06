//
// Created by permal on 10/18/17.
//

#include <thread>
#include <smooth/core/Task.h>
#include <smooth/core/ipc/QueueNotification.h>
#include <smooth/core/timer/ElapsedTime.h>
#include <smooth/core/logging/log.h>


namespace smooth
{
    namespace core
    {
        namespace ipc
        {
            void QueueNotification::notify(ITaskEventQueue* queue)
            {
                // It might look like the queue can grow without bounds, but that is not the case
                // as TaskEventQueues only call this method when they have successfully added the
                // data item to their internal queue. As such, the queue can only be as large as
                // the sum of all queues within the same Task.
                std::unique_lock<std::mutex> lock(guard);
                queues.push(queue);
                cond.notify_one();
            }

            ITaskEventQueue* QueueNotification::wait_for_notification(std::chrono::milliseconds timeout)
            {
                ITaskEventQueue* res = nullptr;

                std::unique_lock<std::mutex> lock(guard);

                if (queues.empty())
                {
                    // Wait until data is available, or timeout. This will atomically release the lock.
                    auto wait_result = cond.wait_until(lock,
                                                       std::chrono::steady_clock::now() + timeout,
                                                       [this]()
                                                       {
                                                           // Stop waiting when there is data
                                                           return !queues.empty();
                                                       });

                    // At this point we will have the lock again.
                    if (wait_result)
                    {
                        if (!queues.empty())
                        {
                            res = queues.front();
                            queues.pop();
                        }
                    }
                }
                else
                {
                    res = queues.front();
                    queues.pop();
                }

                return res;
            }
        }
    }
}
