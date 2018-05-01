// Startfunktion des Programms zur Kontrolle der Relais, die die Rolladenmotoren ansteuern
//

// defines
#define POSIX_SECOND    1000u
#define ONEMINUTE 60
#define ONEHOUR   (60 * 60)
#define MAX_NO_OF_MOTORS    10

#define __USE_MINGW_ANSI_STDIO 0
#define CONFIG_FILE "rolladen_ctrl.cfg"

// includes
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <string>
#include <iostream>
#include <iomanip>

#include "rollershuttermotor.h"
#include "InputCTRL.h"
#include "Parsino.h"

#ifndef NO_IO
#include <wiringPi.h> ///< raspberry pi specific header for GPIOs
#else
#endif

// statics
using namespace std;

// static functions

// static variables
static const char * config_file = CONFIG_FILE;
static uint16_t no_of_motors = 0u;  // no motors
static bool retParser = false;
static Parsino parser;


// functions
/*
 *
 */
int main(void)
{
    // Declaration
    int result = EXIT_SUCCESS; // no error

    // Filling fields from config file
    try
    {
        parser.loadFile(config_file);
        retParser = (uint16_t) parser.get_integer("NUMBER_OF_MOTORS",
                (int *) &no_of_motors);
        std::cout << "Number of motors:" << no_of_motors << std::endl;
    } catch (int e)
    {
        std::cerr << "Parse error at " << std::endl;
        return (EXIT_FAILURE);
    }

    if (no_of_motors != 0u)
    {
#ifndef NO_IO
        wiringPiSetup();
#endif

        /**
         * Initialization of shutter motors
         */
        roller_shutter_motor * motors_p[no_of_motors];       // an array of no_of_motors pointers to "roller_shutter_motor" objects
        InputCTRL * input_p;
        std::string tempString;
        std::string number_string;
        uint16_t tmp_open = 0;
        uint16_t tmp_close = 0;
        time_t open_from = 0;
        time_t open_till = 0;
        uint16_t jj_u16 = 0;

        for (uint16_t ii_u16 = 0; ii_u16 < no_of_motors; ii_u16++)
        {
            jj_u16 = ii_u16 + 1;
            open_till = 0;
            open_from = 0;
            tmp_close = 0;
            tmp_open = 0;
            tempString.clear();
            number_string.clear();

            number_string = std::to_string(jj_u16);

            tempString = "PIN_CLOSE" + number_string;
            if (parser.get_integer(tempString, (int *) &tmp_close))
            {
                std::cout << "Error detected: " << retParser << std::endl;
                break;
            }
            else
            {
            }
            std::cout << tempString << ": " << tmp_close << std::endl;

            tempString = "PIN_OPEN" + number_string;
            if (parser.get_integer(tempString, (int *) &tmp_open))
            {
                std::cout << "Error detected: " << retParser << std::endl;
                break;
            }
            else
            {
            }
            std::cout << tempString << ": " << tmp_open << std::endl;

            tempString = "TIME_CLOSE" + number_string;
            if (parser.get_integer(tempString, (int *) &open_till))
            {
                std::cout << "Error detected: " << retParser << std::endl;
                break;
            }
            else
            {
            }
            std::cout << tempString << ": " << open_till << std::endl;

            tempString = "TIME_OPEN" + number_string;
            if (parser.get_integer(tempString, (int *) &open_from))
            {
                std::cout << "Error detected: " << retParser << std::endl;
                break;
            }
            else
            {
            }
            std::cout << tempString << ": " << open_from << std::endl;

            motors_p[ii_u16] = new (nothrow) roller_shutter_motor(tmp_open,
                    tmp_close, open_from, open_till);
            std::cout << "Motor" << ii_u16 << " initialized successful"
                    << std::endl;
        }
        // Configuration of motors completed

        tempString.clear();
        uint16_t input_pin;
        tempString = "INPUT_PIN";
        if (parser.get_integer(tempString, (int *) &input_pin))
        {
            std::cout << "Error detected: " << retParser << std::endl;
        }
        else
        {
        }
        std::cout << tempString << ": " << input_pin << std::endl;

        input_p = new (nothrow) InputCTRL(input_pin);
        std::cout << input_p->get_logicstate() << std::endl;

        /**
         *  Operation of motors
         */
        bool last_close_b8 = false, last_input_b8 = false;
        while (1)
        {
            // main loop
            if((input_p->get_logicstate() == true)
                    &&(last_input_b8 == false))
            {
                if(last_close_b8 == true)
                {
                    for(uint8_t ii_u8 = 0u; ii_u8 < no_of_motors;ii_u8++)
                    {
                        motors_p[ii_u8]->open_shutter();
                    }
                    last_close_b8 = false;
                }
                else
                {
                    for(uint8_t ii_u8 = 0u; ii_u8 < no_of_motors;ii_u8++)
                    {
                        motors_p[ii_u8]->close_shutter();
                    }
                    last_close_b8 = true;
                }

                last_input_b8 = true;
            }
            else
            {
                last_input_b8 = input_p->get_logicstate();
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        /**
         * Destructing
         */
        // End of Application
        std::cout << "Message:" << result << std::endl;

        for (uint16_t i = 0; i < no_of_motors; i++)
        {
            delete motors_p[i];
        }
        delete input_p;
    }
    else
    {
        // No motors defined
        std::cout << "Error: No motors defined!" << std::endl;
    }

    std::cout << "roller shutter ctrl inactive!" << std::endl;

    return result;
}

