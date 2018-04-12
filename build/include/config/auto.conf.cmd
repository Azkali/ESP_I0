deps_config := \
	/home/azkali/ESPIO/esp-idf/components/app_trace/Kconfig \
	/home/azkali/ESPIO/esp-idf/components/aws_iot/Kconfig \
	/home/azkali/ESPIO/esp-idf/components/bt/Kconfig \
	/home/azkali/ESPIO/esp-idf/components/esp32/Kconfig \
	/home/azkali/ESPIO/esp-idf/components/esp_adc_cal/Kconfig \
	/home/azkali/ESPIO/esp-idf/components/ethernet/Kconfig \
	/home/azkali/ESPIO/esp-idf/components/fatfs/Kconfig \
	/home/azkali/ESPIO/esp-idf/components/freertos/Kconfig \
	/home/azkali/ESPIO/esp-idf/components/heap/Kconfig \
	/home/azkali/ESPIO/esp-idf/components/libsodium/Kconfig \
	/home/azkali/ESPIO/esp-idf/components/log/Kconfig \
	/home/azkali/ESPIO/esp-idf/components/lwip/Kconfig \
	/home/azkali/ESPIO/esp-idf/components/mbedtls/Kconfig \
	/home/azkali/ESPIO/esp-idf/components/openssl/Kconfig \
	/home/azkali/ESPIO/esp-idf/components/pthread/Kconfig \
	/home/azkali/ESPIO/esp-idf/components/spi_flash/Kconfig \
	/home/azkali/ESPIO/esp-idf/components/spiffs/Kconfig \
	/home/azkali/ESPIO/esp-idf/components/tcpip_adapter/Kconfig \
	/home/azkali/ESPIO/esp-idf/components/wear_levelling/Kconfig \
	/home/azkali/ESPIO/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/azkali/ESPIO/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/azkali/ESPIO/a2dp_sink/main/Kconfig.projbuild \
	/home/azkali/ESPIO/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/azkali/ESPIO/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
