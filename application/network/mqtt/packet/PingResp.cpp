//
// Created by permal on 7/22/17.
//

#include <smooth/application/network/mqtt/packet/PingResp.h>

namespace smooth
{
    namespace application
    {
        namespace network
        {
            namespace mqtt
            {
                namespace packet
                {
                    void PingResp::visit(IPacketReceiver& receiver)
                    {
                        receiver.receive(*this);
                    }
                }
            }
        }
    }
}
