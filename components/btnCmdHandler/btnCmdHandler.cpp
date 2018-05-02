 #include "btnCmdHandler.h"

template <typename Tconfig, class Tinherit>
BtnCmdHandler<Tconfig, Tinherit>::BtnCmdHandler(Tconfig _config){
	ESP_LOGI("tag", "Binding GPIO NUM: %d\n", _config.pulse_button_num);
	ESP_LOGD("tag", "Binding GPIO NUM: %d\n", _config.pulse_button_num);
	config = _config;
}

template <typename Tconfig, class Tinherit>
void BtnCmdHandler<Tconfig, Tinherit>::buttonBind() {
	gpio_num_t pbnum = this->config.pulse_button_num;
	gpio_set_direction(pbnum, GPIO_MODE_INPUT);
	gpio_set_pull_mode(pbnum, GPIO_PULLUP_ONLY);
	gpio_set_intr_type(pbnum, GPIO_INTR_NEGEDGE);

	if(BtnCmdHandler::isrIsInstalled == false){
		BtnCmdHandler::isrIsInstalled = true;
		ESP_LOGI("tag", "Initializing isr service\n");
  		gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
	}


	void (*bindHandle)(Tinherit *) = &Tinherit::handleEventBetter;
	printf("Handler pointer: %p\n", bindHandle);
//	BtnCmdHandler::handleEventBetter(static_cast<Tinherit*>(this));
	gpio_isr_handler_add(
		pbnum,
		(gpio_isr_t)(bindHandle),
		this
	);
}

template <typename Tconfig, class Tinherit>
void BtnCmdHandler<Tconfig, Tinherit>::handleEventBetter(Tinherit *_this) {
	ESP_LOGI("tag", "Tiggered event on button: %d\n", _this->config.pulse_button_num);
	//printf("Handler pointer: %p\n", (void*)&_this->handleEvent);
	_this->handleEvent();
}


template <typename Tconfig, class Tinherit>
bool BtnCmdHandler<Tconfig, Tinherit>::isrIsInstalled = false;


SimpleBtnCmdHandler::SimpleBtnCmdHandler(gpio_num_t buttonNum)
: BtnCmdHandler<BtnCmdHandlerConfig, SimpleBtnCmdHandler> (generateConfig(buttonNum))
{
}

BtnCmdHandlerConfig SimpleBtnCmdHandler::generateConfig(gpio_num_t buttonNum) {
	BtnCmdHandlerConfig config = {
		buttonNum
	};
	return config;
}
