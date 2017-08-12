//
// Created by permal on 7/15/17.
//

#pragma once

#include <vector>
#include <bitset>
#include <smooth/core/network/IPacketAssembly.h>
#include <smooth/core/network/IPacketDisassembly.h>
#include <smooth/application/network/mqtt/MQTTProtocolDefinitions.h>
#include <smooth/application/network/mqtt/packet/PacketIdentifierFactory.h>
#include "IPacketReceiver.h"

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
                    typedef std::bitset<8> ByteSet;

                    class MQTTPacket
                            : public smooth::core::network::IPacketAssembly,
                              public smooth::core::network::IPacketDisassembly
                    {
                        public:
                            MQTTPacket() = default;
                            MQTTPacket& operator=(const MQTTPacket&) = default;

                            MQTTPacket(const MQTTPacket& other)
                            {
                                *this = other;
                                calculate_remaining_length_and_variable_header_offset();
                            }

                              // Must return the number of bytes the packet wants to fill
                            // its internal buffer, e.g. header, checksum etc. Returned
                            // value will differ depending on how much data already has been provided.
                            int get_wanted_amount() override;

                            // Used by the underlying framework to notify the packet that {length} bytes
                            // has been written to the buffer pointed to by get_write_pos().
                            // During the call to this method the packet should do whatever it needs to
                            // evaluate if it needs more data or if it is complete.
                            void data_received(int length) override;

                            // Must return the current write position of the internal buffer.
                            // Must point to a buffer than can accept the number of bytes returned by
                            // get_wanted_amount().
                            uint8_t* get_write_pos() override;

                            // Must return true when the packet has received all data it needs
                            // to fully assemble.
                            bool is_complete() override;

                            // Must return true whenever the packet is unable to correctly assemble
                            // based on received data.
                            bool is_error() override;

                            // Must return the total amount of bytes to send
                            int get_send_length() override;
                            // Must return a pointer to the data to be sent.
                            const uint8_t* get_data() override;

                            const std::vector<uint8_t>& get_data_as_vector() const;

                            bool is_too_big() const;

                            void dump() const;

                            PacketType get_mqtt_type() const;

                            virtual void visit(IPacketReceiver& receiver);

                            virtual bool has_packet_identifier() const
                            {
                                return false;
                            }

                            virtual uint16_t get_packet_identifier()
                            {
                                return 0;
                            }

                            int get_send_retry_count() const
                            {
                                return send_retry_count;
                            }

                            void inc_send_retry_count()
                            {
                                ++send_retry_count;
                            };

                        protected:
                            void set_header(PacketType type, uint8_t flags);

                            void encode_remaining_length(int length);

                            void append_string(const std::string& str, std::vector<uint8_t>& target);
                            void append_msb_lsb(uint16_t value, std::vector<uint8_t>& target);
                            void append_data(const uint8_t* data, int length, std::vector<uint8_t>& target);
                            void apply_variable_header(const std::vector<uint8_t>& variable);

                            std::vector<uint8_t> packet{};
                            int VARIABLE_HEADER_OFFSET = 2;
                            int calculated_variable_header_offset = 0;
                            int calculate_remaining_length_and_variable_header_offset();
                            std::string get_string(int offset) const;

                        private:
                            enum ReadingHeaderSection
                            {
                                START = 1,
                                REMAINING_LENGTH,
                                DATA,
                                TOO_BIG
                            };
                            static const int REMAINING_LENGTH_OFFSET = 1;
                            ReadingHeaderSection state = ReadingHeaderSection::START;
                            int bytes_received = 0;
                            int current_length = 0;
                            int wanted_amount = 1;
                            int received_header_length = 0;
                            bool error = false;
                            bool too_big = false;
                            int send_retry_count = 0;
                    };
                }
            }
        }
    }
}