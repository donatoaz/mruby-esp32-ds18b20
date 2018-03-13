/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
     along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <mruby.h>
#include <mruby/value.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "rom/ets_sys.h"
#include "ds18b20.h"
#include "esp_log.h"

#define TAG "MRB_ESP32_DS18B20"

int DS_GPIO;
int init=0;

/// Sends one bit to bus
void ds18b20_send(char bit){
  gpio_set_direction(DS_GPIO, GPIO_MODE_OUTPUT);
  gpio_set_level(DS_GPIO,0);
  ets_delay_us(5);
  if(bit==1)gpio_set_level(DS_GPIO,1);
  ets_delay_us(80);
  gpio_set_level(DS_GPIO,1);
}
// Reads one bit from bus
unsigned char ds18b20_read(void){
  unsigned char PRESENCE=0;
  gpio_set_direction(DS_GPIO, GPIO_MODE_OUTPUT);
  gpio_set_level(DS_GPIO,0);
  ets_delay_us(2);
  gpio_set_level(DS_GPIO,1);
  ets_delay_us(15);
  gpio_set_direction(DS_GPIO, GPIO_MODE_INPUT);
  if(gpio_get_level(DS_GPIO)==1) PRESENCE=1; else PRESENCE=0;
  return(PRESENCE);
}
// Sends one byte to bus
void ds18b20_send_byte(char data){
  unsigned char i;
  unsigned char x;
  for(i=0;i<8;i++){
    x = data>>i;
    x &= 0x01;
    ds18b20_send(x);
  }
  ets_delay_us(100);
}
// Reads one byte from bus
unsigned char ds18b20_read_byte(void){
  unsigned char i;
  unsigned char data = 0;
  for (i=0;i<8;i++)
  {
    if(ds18b20_read()) data|=0x01<<i;
    ets_delay_us(15);
  }
  return(data);
}
// Sends reset pulse
unsigned char ds18b20_RST_PULSE(void){
  unsigned char PRESENCE;
  gpio_set_direction(DS_GPIO, GPIO_MODE_OUTPUT);
  gpio_set_level(DS_GPIO,0);
  ets_delay_us(500);
  gpio_set_level(DS_GPIO,1);
  gpio_set_direction(DS_GPIO, GPIO_MODE_INPUT);
  ets_delay_us(30);
  if(gpio_get_level(DS_GPIO)==0) PRESENCE=1; else PRESENCE=0;
  ets_delay_us(470);
  if(gpio_get_level(DS_GPIO)==1) PRESENCE=1; else PRESENCE=0;
  return PRESENCE;
}
// Returns temperature from sensor
float ds18b20_get_temp(void) {
  if(init==1){
    unsigned char check;
    char temp1=0, temp2=0;
      check=ds18b20_RST_PULSE();
      if(check==1)
      {
        ds18b20_send_byte(0xCC);
        ds18b20_send_byte(0x44);
        vTaskDelay(750 / portTICK_RATE_MS);
        check=ds18b20_RST_PULSE();
        ds18b20_send_byte(0xCC);
        ds18b20_send_byte(0xBE);
        temp1=ds18b20_read_byte();
        temp2=ds18b20_read_byte();
        check=ds18b20_RST_PULSE();
        float temp=0;
        temp=(float)(temp1+(temp2*256))/16;
        return temp;
      }
      else{return 0;}

  }
  else{return 0;}
}

static mrb_value mrb_esp32_ds18b20_get_temp(mrb_state *mrb, mrb_value self) {
  float temp = -1.0;
  temp = ds18b20_get_temp();
  ESP_LOGI(TAG, "Temperature from pin %d: %0.1f\n", DS_GPIO, temp);
  return mrb_float_value(mrb, temp);
}

static mrb_value mrb_esp32_ds18b20_init(mrb_state *mrb, mrb_value self){
  // expects an integer
  mrb_value pin;
  
  mrb_get_args(mrb, "o", &pin);

  // return nil if user did not provide an adequately typed value
  if (!mrb_fixnum_p(pin)) {
    return mrb_nil_value();
  }

  DS_GPIO = mrb_fixnum(pin);
  ESP_LOGI(TAG, "DS_GPIO pin was set to %d", DS_GPIO);

  gpio_pad_select_gpio(DS_GPIO);
  init = 1;

  return self;
}

void mrb_mruby_esp32_ds18b20_gem_init(mrb_state* mrb) {
  struct RClass *esp32, *ds18b20;

  esp32 = mrb_define_module(mrb, "ESP32");

  ds18b20 = mrb_define_module_under(mrb, esp32, "DS18B20");
  mrb_define_module_function(mrb, ds18b20, "ds18b20_init", mrb_esp32_ds18b20_init, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, ds18b20, "ds18b20_get_temp", mrb_esp32_ds18b20_get_temp, MRB_ARGS_REQ(0));
}

// destructor like function, required by mruby
void mrb_mruby_esp32_ds18b20_gem_final(mrb_state* mrb) {
}
