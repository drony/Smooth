//
// Created by permal on 9/1/17.
//

#pragma once

#include "Input.h"
#include <smooth/core/ipc/IISRTaskEventQueue.h>

namespace smooth
{
    namespace core
    {
        namespace io
        {
            class InterruptInputEvent
            {
                public:
                    InterruptInputEvent() = default;

                    InterruptInputEvent(gpio_num_t io, bool state)
                            : io(io), state(state)
                    {
                    }

                    bool get_state() const
                    {
                        return state;
                    }

                    gpio_num_t get_io() const
                    {
                        return io;
                    }

                private:
                    gpio_num_t io;
                    bool state;
            };

            class InterruptInput
                    : private Input
            {
                public:

                    /// Constructs an InterruptInput
                    /// \param queue The queue to put events on
                    /// \param io GPIO pin number
                    /// \param pull_up Set to true if the input has a pull-up (also enables the internal pull up)
                    /// \param pull_down Set to true if the input has a pull-down (also enables the internal pull-down)
                    /// \param interrupt_trigger When the interrupt should trigger
                    InterruptInput(core::ipc::IISRTaskEventQueue<InterruptInputEvent>& queue, gpio_num_t io,
                                   bool pull_up,
                                   bool pull_down,
                                   gpio_int_type_t interrupt_trigger
                    );

                    void update();

                    gpio_num_t get_io() const
                    {
                        return io;
                    }

                private:
                    core::ipc::IISRTaskEventQueue<InterruptInputEvent>& queue;
            };
        }
    }
}
