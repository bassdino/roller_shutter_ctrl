/** InputCTRL.h
 *
 *  Created on: 06.03.2018
 *      Author: Michael
 */

#ifndef INPUTCTRL_H_
#define INPUTCTRL_H_

#define INPUT_ON      1
#define INPUT_OFF     0

#include <cstdint>
#include <thread>         // std::thread

/** Brief description which ends at this dot.
 *  Details follow here.
 */
class InputCTRL
{
private:
    uint16_t gpio_input;     /// gpio read for input
    bool shutoff;   /// Stop reading input
    bool input_active;
    std::thread update_thread;  /// Thread for update of outputs

    void read_input(void);
public:
    InputCTRL();
    InputCTRL(uint16_t input_pin);
    virtual ~InputCTRL();
    bool get_logicstate(void);
};

#endif /* INPUTCTRL_H_ */
