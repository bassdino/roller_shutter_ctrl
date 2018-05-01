/*
 * rollershuttermotor.h
 *
 *  Created on: 30.01.2017
 *      Author: Michael
 */

#ifndef ROLLERSHUTTERMOTOR_H_
#define ROLLERSHUTTERMOTOR_H_

#define MOTOR_RELAY_ON      0
#define MOTOR_RELAY_OFF     1

#include <cstdint>
#include <ctime>
#include <thread>         // std::thread

class roller_shutter_motor final
{
private:
    uint16_t gpio_open;     /// gpio to open the shutter
    uint16_t gpio_close;    /// gpio to close the shutter
    time_t open_from;   /// time when the shutter should be opened
    time_t open_till;   /// time when the shutter should be closed
    time_t driving_endurance;   /// endurance to open or close the shutter
    bool last_close; /// Last position was going to be closed
    bool override_close;    /// override close was activated
    bool override_open;     /// override open was activated
    bool shutoff;   /// Stop processing of this motor
    std::thread update_thread;  /// Thread for update of outputs

    void update_shutter(void);
    void init_pins( void );

public:
    roller_shutter_motor(void);
    roller_shutter_motor(uint16_t open_contact, uint16_t close_contact, time_t from, time_t until);
    virtual ~roller_shutter_motor();
    void init_pins(uint16_t open_contact, uint16_t close_contact, time_t from, time_t until);
    void open_shutter(void);
    void close_shutter(void);
};

#endif /* ROLLERSHUTTERMOTOR_H_ */
