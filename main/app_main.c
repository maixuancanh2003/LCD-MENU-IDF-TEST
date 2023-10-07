#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "rom/uart.h"
#include <string.h>
#include "smbus.h"
#include "i2c-lcd1602.h"
int button0 , button1,button2,button3,button4,button5 ; 
#define TAG "app"
int MODE1_FRONT,MODE2_FRONT ; 
// LCD1602
#define LCD_NUM_ROWS               2
#define LCD_NUM_COLUMNS            32
#define LCD_NUM_VISIBLE_COLUMNS    16

// LCD2004
//#define LCD_NUM_ROWS               4
//#define LCD_NUM_COLUMNS            40
//#define LCD_NUM_VISIBLE_COLUMNS    20

// Undefine USE_STDIN if no stdin is available (e.g. no USB UART) - a fixed delay will occur instead of a wait for a keypress.
#define USE_STDIN  1
//#undef USE_STDIN
#define ESP_INTR_FLAG_DEFAULT 0
#define CONFIG_BUTTON_PIN 0
TaskHandle_t ISR = NULL;

// interrupt service routine, called when the button is pressed
void IRAM_ATTR button_isr_handler(void* arg) {
  
 xTaskResumeFromISR(ISR);
//portYIELD_FROM_ISR(  );
}

// task that will react to button clicks
 void button_task(void *arg)
{
bool led_status = false;
 while(1){  
vTaskSuspend(NULL);
      // led_status = !led_status;
      // gpio_set_level(CONFIG_LED_PIN, led_status);

button0++ ; 

if (button0 == 5) {
  button0 = 1 ; 

}
printf("Button 0 pressed %d\n",button0);
 }
}///////////////////////////////////////////////////////////////////


#define I2C_MASTER_NUM           I2C_NUM_0
#define I2C_MASTER_TX_BUF_LEN    0                     // disabled
#define I2C_MASTER_RX_BUF_LEN    0                     // disabled
#define I2C_MASTER_FREQ_HZ       100000
#define I2C_MASTER_SDA_IO        CONFIG_I2C_MASTER_SDA
#define I2C_MASTER_SCL_IO        CONFIG_I2C_MASTER_SCL

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BUTTON1 15
#define BUTTON2 23
#define BUTTON3 12 
#define BUTTON4 5
#define BUTTON5 13 

void gpio_setup() {
  
gpio_config_t GPIO_config1 ={};
gpio_config_t GPIO_config2 ={};
gpio_config_t GPIO_config3 ={};
gpio_config_t GPIO_config4 ={};
gpio_config_t GPIO_config5 ={};
    GPIO_config1.pin_bit_mask = 1 << BUTTON1;        
    GPIO_config1.mode =GPIO_MODE_INPUT;               /*!< GPIO mode: set input/output mode                     */
    GPIO_config1.pull_up_en  =GPIO_PULLUP_ENABLE;       /*!< GPIO pull-up                                         */
    GPIO_config1.pull_down_en = GPIO_PULLDOWN_DISABLE ;   /*!< GPIO pull-down                                       */
    GPIO_config1.intr_type =GPIO_INTR_DISABLE ;     
  gpio_config(&GPIO_config1) ; 
    GPIO_config2.pin_bit_mask = 1 << BUTTON2;        
    GPIO_config2.mode =GPIO_MODE_INPUT;               /*!< GPIO mode: set input/output mode                     */
    GPIO_config2.pull_up_en  =GPIO_PULLUP_ENABLE;       /*!< GPIO pull-up                                         */
    GPIO_config2.pull_down_en = GPIO_PULLDOWN_DISABLE ;   /*!< GPIO pull-down                                       */
    GPIO_config2.intr_type =GPIO_INTR_DISABLE ;     
  gpio_config(&GPIO_config2) ; 

    GPIO_config3.pin_bit_mask = 1 << BUTTON3;        
    GPIO_config3.mode =GPIO_MODE_INPUT;               /*!< GPIO mode: set input/output mode                     */
    GPIO_config3.pull_up_en  =GPIO_PULLUP_ENABLE;       /*!< GPIO pull-up                                         */
    GPIO_config3.pull_down_en = GPIO_PULLDOWN_DISABLE ;   /*!< GPIO pull-down                                       */
    GPIO_config3.intr_type =GPIO_INTR_DISABLE ;     
  gpio_config(&GPIO_config3) ; 
      GPIO_config4.pin_bit_mask = 1 << BUTTON4;        
    GPIO_config4.mode =GPIO_MODE_INPUT;               /*!< GPIO mode: set input/output mode                     */
    GPIO_config4.pull_up_en  =GPIO_PULLUP_ENABLE;       /*!< GPIO pull-up                                         */
    GPIO_config4.pull_down_en = GPIO_PULLDOWN_DISABLE ;   /*!< GPIO pull-down                                       */
    GPIO_config4.intr_type =GPIO_INTR_DISABLE ;     
  gpio_config(&GPIO_config4) ; 
      GPIO_config5.pin_bit_mask = 1 << BUTTON5;        
    GPIO_config5.mode =GPIO_MODE_INPUT;               /*!< GPIO mode: set input/output mode                     */
    GPIO_config5.pull_up_en  =GPIO_PULLUP_ENABLE;       /*!< GPIO pull-up                                         */
    GPIO_config5.pull_down_en = GPIO_PULLDOWN_DISABLE ;   /*!< GPIO pull-down                                       */
    GPIO_config5.intr_type =GPIO_INTR_DISABLE ;     
  gpio_config(&GPIO_config5) ; 
}

static void i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_DISABLE;  // GY-2561 provides 10kΩ pullups
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_DISABLE;  // GY-2561 provides 10kΩ pullups
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode,
                       I2C_MASTER_RX_BUF_LEN,
                       I2C_MASTER_TX_BUF_LEN, 0);
}

// uart_rx_one_char_block() causes a watchdog trigger, so use the non-blocking
int dem = 0 ; 
// uart_rx_one_char() and delay briefly to reset the watchdog.
int _wait_for_user1()
{
       int s=1;
       dem = 0 ; 
    while (s)
    {
       vTaskDelay(1);
       dem++ ; 
       printf ("time = %d\n",dem); 
       if (gpio_get_level(BUTTON1) == 0 ) {
        s=0 ; 
       }
      //  if (dem >= 200 ) {
      //   s=0 ;  
      //  }
    }
    return 1 ; 
}
static uint8_t _wait_for_user(void)
{
    uint8_t c = 0;

#ifdef USE_STDIN
    while (!c)
    {
       STATUS s = uart_rx_one_char(&c);
       if (s == OK) {
          printf("%c", c);
       }
       vTaskDelay(1);
    }
#else
    vTaskDelay(1000 / portTICK_RATE_MS);
#endif
    return c;
}

void lcd1602_task(void * pvParameter)
{
 
    // Set up I2C
    i2c_master_init();
    i2c_port_t i2c_num = I2C_MASTER_NUM;
    uint8_t address = CONFIG_LCD1602_I2C_ADDRESS;

    // Set up the SMBus
    smbus_info_t * smbus_info = smbus_malloc();
    ESP_ERROR_CHECK(smbus_init(smbus_info, i2c_num, address));
    ESP_ERROR_CHECK(smbus_set_timeout(smbus_info, 1000 / portTICK_RATE_MS));

    // Set up the LCD1602 device with backlight off
    i2c_lcd1602_info_t * lcd_info = i2c_lcd1602_malloc();
    ESP_ERROR_CHECK(i2c_lcd1602_init(lcd_info, smbus_info, true,
                                     LCD_NUM_ROWS, LCD_NUM_COLUMNS, LCD_NUM_VISIBLE_COLUMNS));

    ESP_ERROR_CHECK(i2c_lcd1602_reset(lcd_info));

while (1) {
  loop:
    // printf ("BUTTON 1 = %d\n",button1);
    // printf ("------------------\n");
    // printf ("BUTTON 2 = %d\n",button2);
    // printf ("------------------\n");
    // printf ("BUTTON 3 = %d\n",button3);
    // printf ("------------------\n");
    // vTaskDelay(20);
    // printf ("BUTTON 4 = %d\n",button2);
  if (!gpio_get_level(BUTTON1) ){
  

     button1 ++ ; 
     if (button1 == 3 ) {button1 = 0 ; }
    }
    if (!gpio_get_level(BUTTON2) ){

     button2 ++ ; 
     if (button2 == 3 ) {button2 = 0 ; }
    }
        if (!gpio_get_level(BUTTON3) ){

     button3 ++ ; 
     if (button3 == 3 ) {button3 = 0 ; }
    }
    // trang thai ban dau 
  
///// TEST THỬ SOURCE NGẮT //////
    if (button0 == 2) {
      // printf ("MODE2_FRONT\n");
 i2c_lcd1602_move_cursor(lcd_info, 0, 0);
 i2c_lcd1602_write_string(lcd_info, " LUA CHON MA DU AN");
  i2c_lcd1602_move_cursor(lcd_info, 0, 1);
 i2c_lcd1602_write_string(lcd_info, ">LUA CHON MA CHI TIET"); 

 if (button2 == 1) {
      while (1) {
        // printf ("button2 = %d\n",button2);
 i2c_lcd1602_move_cursor(lcd_info, 0, 0);
 i2c_lcd1602_write_string(lcd_info, ">tb5511        ");
  i2c_lcd1602_move_cursor(lcd_info, 0, 1);
 i2c_lcd1602_write_string(lcd_info, " l298d         "); 
         if (!gpio_get_level(BUTTON2)) {
goto loop ; 
    }
      }

    }
    }

    //////////////////////////////////////////

    /// SOURCE BUTTON0 
    if(button0 == 0 ){
  //  ESP_LOGI(TAG, "display alphabet from 0,0");  // should overflow to second line at "ABC..."
   // _wait_for_user();
    i2c_lcd1602_home(lcd_info);
 i2c_lcd1602_move_cursor(lcd_info, 0, 0);
 i2c_lcd1602_write_string(lcd_info, "THUC HIEN GIA CONG");
  i2c_lcd1602_move_cursor(lcd_info, 0, 1);
 i2c_lcd1602_write_string(lcd_info, " KET THUC GIA CONG"); 
    ESP_LOGI(TAG, "chay sang trai ");
    //_wait_for_user();
    for (int i = 0; i < 16; ++i)
    {
          if (button0 >0  ){
            i2c_lcd1602_clear(lcd_info);
      goto mode1 ; 
    }
        i2c_lcd1602_scroll_display_right(lcd_info);
        vTaskDelay(500 / portTICK_RATE_MS);
        
    }

//  i2c_lcd1602_clear(lcd_info);
//     i2c_lcd1602_set_cursor(lcd_info, true);
//     uint8_t c = 0;
//     uint8_t col = 0;
//     uint8_t row = 0;
//     while (1)
//     {
//       if (_wait_for_user() == 1) {
//         break; 
//       }
//         i2c_lcd1602_write_char(lcd_info, c);
//         vTaskDelay(100 / portTICK_RATE_MS);
//         ESP_LOGD(TAG, "col %d, row %d, char 0x%02x", col, row, c);
//         ++c;
//         ++col;
//         if (col >= LCD_NUM_VISIBLE_COLUMNS)
//         {
//             ++row;
//             if (row >= LCD_NUM_ROWS)
//             {
//                 row = 0;
//             }
//             col = 0;
//             i2c_lcd1602_move_cursor(lcd_info, col, row);
//         }
//     }

    }
     if (button0 == 1) {
mode1 : 
          // printf ("MODE1_FRONT\n");
 i2c_lcd1602_move_cursor(lcd_info, 0, 0);
 i2c_lcd1602_write_string(lcd_info, ">LUA CHON MA DU AN");
  i2c_lcd1602_move_cursor(lcd_info, 0, 1);
 i2c_lcd1602_write_string(lcd_info, " LUA CHON MA CHI TIET"); 

    if (button2 == 1) {
      while (1) {
        printf ("button2 = %d\n",button2);
 i2c_lcd1602_move_cursor(lcd_info, 0, 0);
 i2c_lcd1602_write_string(lcd_info, ">ESP WROOM 32S");
  i2c_lcd1602_move_cursor(lcd_info, 0, 1);
 i2c_lcd1602_write_string(lcd_info, " RASPERRY PI 3B"); 
         if (!gpio_get_level(BUTTON2)) {
goto loop ; 
    }
      }

    }
    }
    
     if (button0 == 3) {
      // printf ("MODE2_FRONT\n");
 i2c_lcd1602_move_cursor(lcd_info, 0, 0);
 i2c_lcd1602_write_string(lcd_info, ">THUC HIEN GIA CONG");
  i2c_lcd1602_move_cursor(lcd_info, 0, 1);
 i2c_lcd1602_write_string(lcd_info, " KET THUC GIA CONG"); 

 if (button2 == 1) {
      while (1) {
        // printf ("button2 = %d\n",button2);

 i2c_lcd1602_move_cursor(lcd_info, 0, 0);
 i2c_lcd1602_write_string(lcd_info, "   TINH TRANG   ");
  i2c_lcd1602_move_cursor(lcd_info, 0, 1);
 i2c_lcd1602_write_string(lcd_info, "  DANG GIA CONG  "); 
         if (!gpio_get_level(BUTTON2)) {
goto loop ; 
    } }
    }
     }
     if (button0 == 4) {
      // printf ("MODE2_FRONT\n");
 i2c_lcd1602_move_cursor(lcd_info, 0, 0);
 i2c_lcd1602_write_string(lcd_info, " THUC HIEN GIA CONG");
  i2c_lcd1602_move_cursor(lcd_info, 0, 1);
 i2c_lcd1602_write_string(lcd_info, ">KET THUC GIA CONG"); 

 if (button2 == 1) {
      while (1) {
        // printf ("button2 = %d\n",button2);
 char D[20] = "   TINH TRANG   ";
 i2c_lcd1602_move_cursor(lcd_info, 0, 0);
 i2c_lcd1602_write_string(lcd_info, D);
  i2c_lcd1602_move_cursor(lcd_info, 0, 1);
 i2c_lcd1602_write_string(lcd_info, " KET THUC GIA CONG"); 
         if (!gpio_get_level(BUTTON2)) {
goto loop ; 
    } }
    }
     }
}
      
// code test 
//while (1) {
//   tieptuc : 
//  if (1 ){
//  if (!gpio_get_level(BUTTON5) ){
//   vTaskDelay(50/ portTICK_PERIOD_MS);
//     button5 ++ ; 
//     if (button5 == 2) {button5 = 0 ;}
//     }
//  if (button5 == 1) {
//     goto MADUAN_DUNGYEN; 
//  }
//     }
//     if (!gpio_get_level(BUTTON3)) {
//         vTaskDelay(50/ portTICK_PERIOD_MS)
//       button3 ++ ; 
//       if (button3 ==2 ) {
//         button3 = 0 ; 
//       }
//     }
// if (button3 == 1) {
//     goto LUACHONMACHITIET_DUNGYEN;
// }
//     goto LUACHONMADUAN_DUNGYEN ; 
// }
// MACHITIETBENTRONG : 
//      i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">#D32423");
//     i2c_lcd1602_move_cursor(lcd_info, 0, 1);
//     i2c_lcd1602_write_string(lcd_info, " #R78722");
//     goto 
// LUACHONMACHITIET_DUNGYEN :
//      i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, " LUA CHON MA DU AN");
//     i2c_lcd1602_move_cursor(lcd_info, 0, 1);
//     i2c_lcd1602_write_string(lcd_info, ">LUA CHON MA CHI TIET");
//     if (button5 == 1) {
//       goto MACHITIET_BENTRONG ; 
//     }
// goto tieptuc ; 
//  MADUAN_DUNGYEN: 
//      i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">ESP32 WROOM S3");
//     i2c_lcd1602_move_cursor(lcd_info, 0, 1);
//     i2c_lcd1602_write_string(lcd_info, " RASPBERRY PI3B+           ");
//     if (1) 
//     {
//   vTaskDelay(50/ portTICK_PERIOD_MS);
//   if (!gpio_get_level(BUTTON5)) 
//     {
// goto LUACHONMADUAN_DUNGYEN ; 
//     }}
// goto  MADUAN_DUNGYEN ; 
    // _wait_for_user() ; 
  //  THUCHIENGIACONG_CHUYENDONG : 
  //   i2c_lcd1602_write_string(lcd_info, ">THUC HIEN GIA CONG");
  //   vTaskDelay(50) ; 
  //   i2c_lcd1602_move_cursor(lcd_info, 0, 0);
  //   i2c_lcd1602_write_string(lcd_info, ">HUC HIEN GIA CONG         ");
  //   vTaskDelay(50) ; 
  //   i2c_lcd1602_move_cursor(lcd_info, 0, 0);
  //   i2c_lcd1602_write_string(lcd_info, ">UC HIEN GIA CONG         ");
  //   vTaskDelay(50) ; 
  //   i2c_lcd1602_move_cursor(lcd_info, 0, 0);
  //   i2c_lcd1602_write_string(lcd_info, ">C HIEN GIA CONG         ");
  //   vTaskDelay(50) ; 
  //   // GOI HAM LUA CHON MA DU AN 
  //   i2c_lcd1602_move_cursor(lcd_info, 0, 0);
  //   i2c_lcd1602_write_string(lcd_info, "> HIEN GIA CONG         ");
  //   vTaskDelay(50) ;  
  //   i2c_lcd1602_move_cursor(lcd_info, 0, 0);
  //   i2c_lcd1602_write_string(lcd_info, ">HIEN GIA CONG         ");
  //   vTaskDelay(50) ; 
  //   i2c_lcd1602_move_cursor(lcd_info, 0, 0);
  //   i2c_lcd1602_write_string(lcd_info, ">IEN GIA CONG         ");
  //   vTaskDelay(50) ; 
  //   i2c_lcd1602_move_cursor(lcd_info, 0, 0);
  //   i2c_lcd1602_write_string(lcd_info, ">EN GIA CONG         ");
  //   vTaskDelay(50) ;  
  //   i2c_lcd1602_move_cursor(lcd_info, 0, 0);
  //   i2c_lcd1602_write_string(lcd_info, ">N GIA CONG         ");
  //   vTaskDelay(50) ; 
  //   i2c_lcd1602_move_cursor(lcd_info, 0, 0);
  //   i2c_lcd1602_write_string(lcd_info, "> GIA CONG         ");
  //   vTaskDelay(50) ; 
  //   i2c_lcd1602_move_cursor(lcd_info, 0, 0);
  //   i2c_lcd1602_write_string(lcd_info, ">GIA CONG           ");
  //   vTaskDelay(50) ; 
  //   i2c_lcd1602_move_cursor(lcd_info, 0, 0);
  //   i2c_lcd1602_write_string(lcd_info, ">IA CONG         ");
  //   vTaskDelay(50) ; 
  //   i2c_lcd1602_move_cursor(lcd_info, 0, 0);
  //   i2c_lcd1602_write_string(lcd_info, ">A CONG         ");
  //   vTaskDelay(50) ; 
  //   i2c_lcd1602_move_cursor(lcd_info, 0, 0);
  //   i2c_lcd1602_write_string(lcd_info, "> CONG         ");
  //   vTaskDelay(50) ;  
  //   i2c_lcd1602_move_cursor(lcd_info, 0, 0);
  //   i2c_lcd1602_write_string(lcd_info, ">CONG                           ");
  //   vTaskDelay(50) ;  
  //   i2c_lcd1602_move_cursor(lcd_info, 0, 0);
  //   i2c_lcd1602_write_string(lcd_info, ">ONG                           ");
  //   vTaskDelay(50) ; 
  //   i2c_lcd1602_move_cursor(lcd_info, 0, 0);
  //   i2c_lcd1602_write_string(lcd_info, ">NG                                    ");
  //   vTaskDelay(50) ; 
  //   i2c_lcd1602_move_cursor(lcd_info, 0, 0);
  //   i2c_lcd1602_write_string(lcd_info, ">G                                    ");
  //   vTaskDelay(50) ; 
  //   i2c_lcd1602_move_cursor(lcd_info, 0, 0);
  //   i2c_lcd1602_write_string(lcd_info, ">                                     ");
  //   vTaskDelay(50) ; 
   //THUCHIENGIACONG_DUNGYEN:
    // i2c_lcd1602_move_cursor(lcd_info, 0, 0);
    // i2c_lcd1602_write_string(lcd_info, ">THUC HIEN GIA CONG");
    // i2c_lcd1602_move_cursor(lcd_info, 1, 0);
    // i2c_lcd1602_write_string(lcd_info, " KET THUC GIA CONG");
    // _wait_for_user() ; 
    // LUACHONMADUAN_DUNGYEN: 
    // i2c_lcd1602_move_cursor(lcd_info, 0, 0);
    // i2c_lcd1602_write_string(lcd_info, ">LUA CHON MA DU AN");
    // i2c_lcd1602_move_cursor(lcd_info, 0, 1);
    // i2c_lcd1602_write_string(lcd_info, " LUA CHON MA CHI TIET"); 
    // goto tieptuc ; 
//   LUACHONMADUAN_CHUYENDONG: 
// i2c_lcd1602_write_string(lcd_info, ">LUA CHON MA DU AN                 ");
//     i2c_lcd1602_move_cursor(lcd_info, 0, 1);
//     i2c_lcd1602_write_string(lcd_info, " LUA CHON MA CHI TIET"); 
//     vTaskDelay(50) ; 
//     i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">UA CHON MA DU AN                 ");
//     vTaskDelay(50) ; 
//     i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">A CHON MA DU AN                 ");
//     vTaskDelay(50) ; 
//     i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, "> CHON MA DU AN                 ");
//     vTaskDelay(50) ; 
//     i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">CHON MA DU AN                 ");
//     vTaskDelay(50) ;  
//     i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">HON MA DU AN                 ");
//     vTaskDelay(50) ; 
//     i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">ON MA DU AN                 ");
//     vTaskDelay(50) ; 
//     i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">N MA DU AN                 ");
//     vTaskDelay(50) ;  
//     i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, "> MA DU AN                 ");
//     vTaskDelay(50) ; 
//     i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">MA DU AN                 ");
//     vTaskDelay(50) ; 
//     i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">A DU AN                 ");
//     vTaskDelay(50) ; 
//     i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, "> DU AN                 ");
//     vTaskDelay(50) ; 
//     i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">DU AN                 ");
//     vTaskDelay(50) ; 
//     i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">U AN                 ");
//     vTaskDelay(50) ;  
//     i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, "> AN                 ");
//     vTaskDelay(50) ;  
//     i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">AN                 ");
//     vTaskDelay(50) ; 
//     i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">N                 ");
//     vTaskDelay(50) ; 
//     i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">                         ");
//     vTaskDelay(50) ; 
// KETTHUCGIACONG : 
// i2c_lcd1602_write_string(lcd_info, ">KET THUC GIA CONG");
//     vTaskDelay(50) ; 
//     i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">ET THUC GIA CONG                    ");
//     vTaskDelay(50) ; 
//     i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">T THUC GIA CONG                    ");
//     vTaskDelay(50) ; 
//         i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, "> THUC GIA CONG                    ");
//     vTaskDelay(50) ; 
//         i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">THUC GIA CONG                    ");
//     vTaskDelay(50) ; 
//         i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">HUC GIA CONG                    ");
//     vTaskDelay(50) ; 
//         i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">UC GIA CONG                    ");
//     vTaskDelay(50) ; 
//         i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">C GIA CONG                    ");
//     vTaskDelay(50) ; 
//         i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, "> GIA CONG                    ");
//     vTaskDelay(50) ; 
//         i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">GIA CONG                    ");
//     vTaskDelay(50) ; 
//         i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">IA CONG                    ");
//     vTaskDelay(50) ; 
//         i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">A CONG                    ");
//     vTaskDelay(50) ; 
//         i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, "> CONG                    ");
//     vTaskDelay(50) ; 
//         i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">CONG                    ");
//     vTaskDelay(50) ; 
//         i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">ONG                    ");
//     vTaskDelay(50) ; 
//         i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">NG                   ");
//     vTaskDelay(50) ; 
//         i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">G                   ");
//     vTaskDelay(50) ; 
//           i2c_lcd1602_move_cursor(lcd_info, 0, 0);
//     i2c_lcd1602_write_string(lcd_info, ">                   ");
//     vTaskDelay(50) ; 


}

void get_button () {
  
while (1) {

  if (gpio_get_level(BUTTON1) == 0) {
    // printf("BUTTON1 \n"); 
    // printf ("-------\n") ; 
    button1++; 
    vTaskDelay(200/ portTICK_PERIOD_MS);
    if (button1 == 2) {
        button1 = 0 ; 
    }
  }

  if (gpio_get_level(BUTTON2) == 0) {
    // printf("BUTTON2 \n"); 
    // printf ("-------\n") ; 
    button2++; 
    vTaskDelay(200/ portTICK_PERIOD_MS);
  }
    if (button2 == 3) {
        button2 = 0 ; 
    }
  if (gpio_get_level(BUTTON3) == 0) {
    // printf("BUTTON3 \n"); 
    // printf ("-------\n") ; 
    button3++; 
    vTaskDelay(200/ portTICK_PERIOD_MS);
  }
    if (button3 == 2) {
        button3 = 0 ; 
    }
  if (gpio_get_level(BUTTON4) == 0) {
    printf("BUTTON4 \n"); 
    printf ("-------\n") ; 
    button4++; 
    vTaskDelay(200/ portTICK_PERIOD_MS);
  }
    if (button4 == 2) {
        button4 = 0 ; 
    }
}}
void app_main()
{   gpio_setup() ; 

    gpio_pad_select_gpio(CONFIG_BUTTON_PIN);

  
  // set the correct direction
  gpio_set_direction(CONFIG_BUTTON_PIN, GPIO_MODE_INPUT);

  
  // enable interrupt on falling (1->0) edge for button pin
  gpio_set_intr_type(CONFIG_BUTTON_PIN, GPIO_INTR_NEGEDGE);

  
  //Install the driver’s GPIO ISR handler service, which allows per-pin GPIO interrupt handlers.
  // install ISR service with default configuration
  gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
  
  // attach the interrupt service routine
  gpio_isr_handler_add(CONFIG_BUTTON_PIN, button_isr_handler, NULL);
 xTaskCreate( button_task, "button_task", 4096, NULL , 10,&ISR );
 xTaskCreate(&lcd1602_task, "lcd1602_task", 4096, NULL, 5, NULL);


}
    
