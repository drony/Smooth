//
// Created by permal on 8/18/17.
//

#pragma once

#include <smooth/core/io/spi/Master.h>
#include <smooth/core/io/spi/SPIDevice.h>
#include <smooth/core/io/Output.h>

namespace smooth
{
    namespace application
    {
        namespace display
        {
            class ST7735
            {
                public:
                    ST7735(core::io::spi::Master& master);

                    bool initialize();

                private:
                    class ST7735_SPI
                            : public smooth::core::io::spi::SPIDevice
                    {
                        public:
                            ST7735_SPI(gpio_num_t data_command_pin)
                                    : SPIDevice(
                                    0,  // command_bits
                                    0,  // address_bits,
                                    0,  // bits_between_address_and_data_phase,
                                    0,  // spi_mode,
                                    128,// positive_duty_cycle,
                                    1,  //cs_ena_posttrans,
                                    26000000,//clock_speed_hz,
                                    SPI_DEVICE_3WIRE | SPI_DEVICE_HALFDUPLEX,// flags,
                                    7,  // queue_size,
                                    true,
                                    false),
                                      dc(data_command_pin, false, true)
                            {
                            }

                            void pre_transmission_action(spi_transaction_t* trans) override
                            {
                                // TODO dc.set/clr
                            }

                        private:
                            core::io::Output dc;
                    };

                    core::io::spi::Master& master;
                    std::unique_ptr<core::io::spi::ISPIDevice> spi;
            };
        }
    }
}
