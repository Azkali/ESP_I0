#include "btnCmdHandler.h"

template <typename Tconfig, class Tinherit>
BtCmdHandler<Tconfig, Tinherit>::BtCmdHandler(Tconfig _config){
	_config = _config;

	gpio_set_direction((gpio_num_t)_config.pulse_button_num, GPIO_MODE_INPUT);
	gpio_set_pull_mode((gpio_num_t)_config.pulse_button_num, GPIO_PULLUP_ONLY);
	gpio_set_intr_type((gpio_num_t)_config.pulse_button_num, GPIO_INTR_NEGEDGE);

	void (*bindHandle)(Tinherit *) = &BtCmdHandler::handleEventBetter;
	printf("GPIO NUM: %d\n", _config.pulse_button_num);
	gpio_isr_handler_add(
		(gpio_num_t)(_config.pulse_button_num),
		(gpio_isr_t)(bindHandle),
		this
	);
}
template <typename Tconfig, class Tinherit>
void BtCmdHandler<Tconfig, Tinherit>::handleEventBetter(Tinherit *_this) {
	_this->handleEvent();
}