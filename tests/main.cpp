#include "test.h"

#include <stdint.h>
#include <hal/GPIO.h>
#include <hal/System.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

using namespace dhcom;


static const char *strHelp = "dhcom-hal-test usage:"
                             "dhcom-hal-test\n\n";



int main(int argc, char **argv)
{
    System system_ = System();
    System::HARDWARE hw = System::detectSoC();

    system_.setHardware(hw);

    dhcom::GPIO gpio_E(system_, dhcom::GPIO::PORT_GPIO_E);
    dhcom::GPIO gpio_F(system_, dhcom::GPIO::PORT_GPIO_F);
    dhcom::GPIO gpio_H(system_, dhcom::GPIO::PORT_GPIO_H);
    dhcom::GPIO gpio_I(system_, dhcom::GPIO::PORT_GPIO_I);
    dhcom::GPIO *gpios[] = { &gpio_E, &gpio_F, &gpio_H , &gpio_I };

    for (dhcom::GPIO *gpio : gpios) {
        dhcom::STATUS ret;

        ret = gpio->open();
        if(ret != STATUS_SUCCESS) {
            printf("open failed: %d\n", ret);
            exit(EXIT_FAILURE);
        }

        ret = gpio->setDirection(dhcom::GPIO::DIRECTION_INPUT);
        if(ret != STATUS_SUCCESS) {
            printf("set direction input failed: %d\n", ret);
            exit(EXIT_FAILURE);
        }

        bool state = gpio->get(&ret);
        printf("state: %d\n", state);
        if(STATUS_SUCCESS != ret) {
            printf("get gpio failed: %d\n", ret);
            exit(EXIT_FAILURE);
        }

    }

    for (dhcom::GPIO *gpio : gpios) {
        dhcom::STATUS ret;

        ret = gpio->setDirection(dhcom::GPIO::DIRECTION_OUTPUT);
        if(ret != STATUS_SUCCESS) {
            printf("set direction output failed: %d\n", ret);
            exit(EXIT_FAILURE);
        }
    }

    int iters = 100;
    while (iters > 0) {
        int run_steps = 8;
        iters--;
        int masks[] = { 1, 2, 4, 8, 8, 4, 2, 1};
        while (run_steps > 0) {
            run_steps--;
            int mask = masks[run_steps];
            for (dhcom::GPIO *gpio : gpios) {
                dhcom::STATUS ret;

                ret = gpio->set(mask & 0x0001);
                if(ret != STATUS_SUCCESS) {
                    printf("set output failed: %d\n", ret);
                    exit(EXIT_FAILURE);
                }
                usleep(300);
                mask = mask >> 1;
            }
        }
    }


    // close
    for (dhcom::GPIO *gpio : gpios)
        gpio->close();

    exit(EXIT_SUCCESS);
}
