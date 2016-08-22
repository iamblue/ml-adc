#include <string.h>
#include "jerry-api.h"
#include "pinmux.h"
#include "hal_adc.h"
#include "microlattice.h"

static uint16_t adc_to_voltage(uint16_t source_code) {
  uint16_t voltage = (source_code*2500)/4095;
  return voltage;
}

DELCARE_HANDLER(__adcRead) {

  uint16_t data[4] = {0};
  uint16_t voltage[4] = {0};
  uint8_t count = 0;
  uint8_t channel = (int) jerry_get_number_value(args_p[0]);
  int check = 0;

  hal_adc_init();

  while(check < 1) {
    hal_adc_get_data_polling(count, &data[count]);
    voltage[count] = adc_to_voltage(data[count]);
    count++;
    if(count == 4) {
      check = 1;
    }
  }

  hal_adc_deinit();
  return jerry_create_number(voltage[channel]);
}

void ml_adc_init(void) {

  hal_pinmux_set_function(0, 7);
  hal_pinmux_set_function(1, 7);
  hal_pinmux_set_function(2, 7);
  hal_pinmux_set_function(3, 7);

  REGISTER_HANDLER(__adcRead);
}