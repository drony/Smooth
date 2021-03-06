//
// Created by permal on 7/22/17.
//

#include <smooth/application/network/mqtt/packet/Publish.h>

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
                    Publish::Publish(const std::string& topic, const uint8_t* data, int length, QoS qos, bool retain)
                    {
                        core::util::ByteSet flags(0);
                        flags.set(0, retain);
                        flags.set(1, qos & 0x1);
                        flags.set(2, qos & 0x2);
                        set_header(PacketType::PUBLISH, flags);

                        std::vector<uint8_t> variable_header{};
                        // Topic
                        append_string(topic, variable_header);

                        // Packet identifier (can't use has_packet_identifier() since we're not fully constructed yet
                        if (qos > AT_MOST_ONCE)
                        {
                            append_msb_lsb(PacketIdentifierFactory::get_id(), variable_header);
                        }

                        // Payload
                        append_data(data, length, variable_header);

                        apply_constructed_data(variable_header);
                    }

                    std::string Publish::get_topic() const
                    {
                        calculate_remaining_length_and_variable_header_offset();
                        return get_string(get_variable_header_start());
                    }

                    int Publish::get_variable_header_length() const
                    {
                        return get_topic().length()
                               // Add two for length bytes for string
                               + 2
                               // Add two more for optional packet identifier
                               + (has_packet_identifier() ? 2 : 0);
                    }

                    void Publish::visit(IPacketReceiver& receiver)
                    {
                        receiver.receive(*this);
                    }
                }
            }
        }
    }
}
