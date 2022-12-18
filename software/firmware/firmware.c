#include "system.h"
#include "periphs.h"
#include "iob-uart.h"
#include "printf.h"
#include "iob-gpio.h"

int main()
{
   /****************************GPIO***********************/
  gpio_init(GPIO_BASE);
  gpio_set_output_enable(0x01);
  //gpio_set(0x01);
  int a = 0, b = 0;
  a = gpio_get();
  printf("\n GPIO received :%d \n", a);

  // Forever loop: will map the inputs into the outputs
  // To visualize changes init with make sim-build VDC=1
  // Observe the waveforms log
  while(1){
    a = gpio_get();
    //mask the value and make it suitable for outputs
    if((a & 0x02) && (a != b)) {
      gpio_set(0x01);
      printf("\n GPIO received :%d \n", a);
      b = a;
    } else if ((a == 0) && (a != b)){
      gpio_set(0x00);
      printf("\n GPIO received :%d \n", a);
      b = a;
    }
    

  }
 
}
