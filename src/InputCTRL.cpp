/** InputCTRL.cpp
 *
 *  Created on: 06.03.2018
 *      Author: Michael
 */

#include "InputCTRL.h"
#ifndef NO_IO
#include <wiringPi.h> ///< raspberry pi specific header for GPIOs
#else
#endif

#define SAMPLE_CNT  (50u)

InputCTRL::InputCTRL()
{
    this->gpio_input = 0u;
    this->shutoff = false;
    this->input_active = false;
    this->update_thread = std::thread(&InputCTRL::read_input, this);
#ifndef NO_IO
    pinMode (this->gpio_input, INPUT);
#else
    printf("IO initialized\n");
#endif

}

InputCTRL::InputCTRL(uint16_t input_pin)
{
    this->gpio_input = input_pin;
    this->shutoff = false;
    this->input_active = false;
    this->update_thread = std::thread(&InputCTRL::read_input, this);
#ifndef NO_IO
    pinMode (this->gpio_input, INPUT);
#else
    printf("IO initialized\n");
#endif

}


InputCTRL::~InputCTRL()
{
    this->shutoff = true;
    this->update_thread.join();
}

bool InputCTRL::get_logicstate(void)
{
    return this->input_active;
}

void InputCTRL::read_input(void)
{
    static uint16_t input_counter_u16 = 0u;
    static uint16_t sample_counter_u16 = 0u;

    try
    {
        while(this->shutoff == false) // keep running
        {
            if(sample_counter_u16 < SAMPLE_CNT) // read back input
            {
#ifndef NO_IO
                if(digitalRead (this->gpio_input) == HIGH)
                {
                    input_counter_u16++;
                }
                else{} // input counter stays
#endif
                sample_counter_u16++;
            }
            else    // analyze and reset
            {
                if(input_counter_u16 > (SAMPLE_CNT / 8u))
                {
                    this->input_active = true;
                }
                else
                {
                    this->input_active = false;
                }
                sample_counter_u16 = 0u;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    } catch (...)
    {

    }
}
