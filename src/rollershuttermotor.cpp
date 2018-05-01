/*
 * rollershuttermotor.cpp
 *
 *  Created on: 30.01.2017
 *      Author: Michael
 */

#include "rollershuttermotor.h"
#include <cstdio>
#include <thread>
#include <ctime>

#ifndef NO_IO
#include <wiringPi.h> ///< raspberry pi specific header for GPIOs
#else
#endif

/** Default Constructor
 *
 */
roller_shutter_motor::roller_shutter_motor(void)
{
    this->gpio_open = 0;
    this->gpio_close = 0;
    this->open_from = 0;
    this->open_till = 0;
    this->driving_endurance = 3;
    this->last_close = false;
    this->override_close = false;
    this->override_open = false;
    this->shutoff = false;
    printf("default rollershuttermotor without IO constructed \n");

    update_thread = std::thread(&roller_shutter_motor::update_shutter, this);
}

/** Constructor with inputs
 *
 */
roller_shutter_motor::roller_shutter_motor(uint16_t open_contact,
        uint16_t close_contact, time_t from, time_t until)
{
    this->gpio_open = open_contact;
    this->gpio_close = close_contact;
    this->open_from = from;
    this->open_till = until;
    this->driving_endurance = 3;
    this->last_close = false;
    this->override_close = false;
    this->override_open = false;
    this->shutoff = false;

    init_pins();
    printf("rollershuttermotor constructed \n");

    update_thread = std::thread(&roller_shutter_motor::update_shutter, this);
}

roller_shutter_motor::~roller_shutter_motor()
{
    this->shutoff = true;
    update_thread.join();
    printf("rollershuttermotor deconstructed \n");
}

/**
 *
 */
void roller_shutter_motor::init_pins()
{
#ifndef NO_IO
    digitalWrite(this->gpio_open, MOTOR_RELAY_OFF);
    digitalWrite(this->gpio_close, MOTOR_RELAY_OFF);
    pinMode (this->gpio_open, OUTPUT);
    pinMode (this->gpio_open, OUTPUT);
#else
    printf("IO initialized\n");
#endif
}

/**
 *
 */
void roller_shutter_motor::init_pins(uint16_t open_contact,
        uint16_t close_contact, time_t from, time_t until)
{
    this->gpio_open = open_contact;
    this->gpio_close = close_contact;
    this->open_from = from;
    this->open_till = until;

#ifndef NO_IO
    digitalWrite(this->gpio_open, MOTOR_RELAY_OFF);
    digitalWrite(this->gpio_close, MOTOR_RELAY_OFF);
    pinMode (this->gpio_open, OUTPUT);
    pinMode (this->gpio_open, OUTPUT);
#else
    printf("IO initialized\n");
#endif
}

/**
 *
 */
void roller_shutter_motor::update_shutter(void)
{
    time_t recTime;
    int recHour, openHour, closeHour;
    int recMin, openMin, closeMin;

    openHour = gmtime(&this->open_from)->tm_hour;
    closeHour = gmtime(&this->open_till)->tm_hour;
    openMin = gmtime(&this->open_from)->tm_min;
    closeMin = gmtime(&this->open_till)->tm_min;

    try
    {
        while (this->shutoff == false)
        {
            time(&recTime);  /* get current time; same as: timer = time(NULL)  */
            recHour = localtime(&recTime)->tm_hour;
            recMin = localtime(&recTime)->tm_min;

            if (this->last_close == true)
            {
                if ((this->override_open == true)
                        ||(recHour == openHour && recMin == openMin))
                {
#ifndef NO_IO
                    digitalWrite(this->gpio_open, MOTOR_RELAY_OFF);
                    digitalWrite(this->gpio_close, MOTOR_RELAY_ON);
#endif
                    printf("rollershuttermotor is going to close \n");
                    this->override_close = false;

                    std::this_thread::sleep_for(
                            std::chrono::seconds(
                                    (uint16_t) this->driving_endurance));
                }
                else
                {
#ifndef NO_IO
                    digitalWrite(this->gpio_open, MOTOR_RELAY_OFF);
                    digitalWrite(this->gpio_close, MOTOR_RELAY_OFF);
#endif
                    std::this_thread::sleep_for(std::chrono::seconds(10));
                }
            }
            else // shutter went open last time
            {
                if ((this->override_close == true)
                    ||(recHour == closeHour && recMin == closeMin))
                {
#ifndef NO_IO
                    digitalWrite(this->gpio_open, MOTOR_RELAY_ON);
                    digitalWrite(this->gpio_close, MOTOR_RELAY_OFF);
#endif
                    printf("rollershuttermotor is going to open \n");
                    this->override_open = false;

                    std::this_thread::sleep_for(
                            std::chrono::seconds(
                                    (uint16_t) this->driving_endurance));
                }
                else
                {
#ifndef NO_IO
                    digitalWrite(this->gpio_open, MOTOR_RELAY_OFF);
                    digitalWrite(this->gpio_close, MOTOR_RELAY_OFF);
#endif
                    std::this_thread::sleep_for(std::chrono::seconds(10));
                }
            }
        }
#ifndef NO_IO
        digitalWrite(this->gpio_open, MOTOR_RELAY_OFF);
        digitalWrite(this->gpio_close, MOTOR_RELAY_OFF);
#endif
        printf("rollershuttermotor in shutoff \n");
    } catch (...)
    {
#ifndef NO_IO
        digitalWrite(this->gpio_open, MOTOR_RELAY_OFF);
        digitalWrite(this->gpio_close, MOTOR_RELAY_OFF);
#endif
        printf("rollershuttermotor catched and now in shutoff \n");
    }

}

/**
 *
 */
void roller_shutter_motor::open_shutter()
{
    this->override_open = true;
}

void roller_shutter_motor::close_shutter()
{
    this->override_close = true;
}

