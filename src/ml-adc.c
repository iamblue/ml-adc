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

  uint16_t data[1] = {0};
  uint16_t voltage[1] = {0};
  uint8_t channel = (int) jerry_get_number_value(args_p[0]);

  hal_adc_init();
  hal_adc_get_data_polling(channel, &data[1]);
  voltage[1] = adc_to_voltage(data[1]);
  hal_adc_deinit();
  return jerry_create_number(voltage[1]);
}

void ml_adc_init(void) {

  hal_pinmux_set_function(0, 7);
  hal_pinmux_set_function(1, 7);
  hal_pinmux_set_function(2, 7);
  hal_pinmux_set_function(3, 7);

  REGISTER_HANDLER(__adcRead);
}