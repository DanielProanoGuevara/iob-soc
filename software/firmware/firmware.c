#include "system.h"
#include "periphs.h"
#include "iob-uart.h"
#include "printf.h"
#include "iob-gpio.h"

char *send_string = "Sending this string as a file to console.\n"
                    "The file is then requested back from console.\n"
                    "The sent file is compared to the received file to confirm " 
                    "correct file transfer via UART using console.\n"
                    "Generating the file in the firmware creates an uniform "
                    "file transfer between pc-emul, simulation and fpga without"
                    " adding extra targets for file generation.\n";

// copy src to dst
// return number of copied chars (excluding '\0')
int string_copy(char *dst, char *src) {
    if (dst == NULL || src == NULL) {
        return -1;
    }
    int cnt = 0;
    while(src[cnt] != 0){
        dst[cnt] = src[cnt];
        cnt++;
    }
    dst[cnt] = '\0';
    return cnt;
}

// 0: same string
// otherwise: different
int compare_str(char *str1, char *str2, int str_size) {
    int c = 0;
    while(c < str_size) {
        if (str1[c] != str2[c]){
            return str1[c] - str2[c];
        }
        c++;
    }
    return 0;
}

int main()
{
  //init uart
  uart_init(UART_BASE,FREQ/BAUD);

  /*
  //test puts
  uart_puts("\n\n\nHello world!\n\n\n");

  //test printf with floats 
  printf("Value of Pi = %f\n\n", 3.1415);
  */

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
  

  /* //print first 100 values of Fibonacci
  unsigned long t1 = 0, t2 = 1;
  unsigned long next = t1 + t2;
  int i;

  printf("%d \n", t1);

  printf("%d \n", t2);

  for(i=0; i <= 97; i++){
    printf("%d \n", next);
    t1 = t2;
    t2 = next;
    next = t1 + t2;   
  }
  */
 
  
  
  

  //test file send
  char *sendfile = malloc(1000);
  int send_file_size = 0;
  send_file_size = string_copy(sendfile, send_string);
  uart_sendfile("Sendfile.txt", send_file_size, sendfile);

  //test file receive
  char *recvfile = malloc(10000);
  int file_size = 0;
  file_size = uart_recvfile("Sendfile.txt", recvfile);

  //compare files
  if (compare_str(sendfile, recvfile, send_file_size)) {
      printf("FAILURE: Send and received file differ!\n");
  } else {
      printf("SUCCESS: Send and received file match!\n");
  }

  free(sendfile);
  free(recvfile);

  uart_finish();
}
