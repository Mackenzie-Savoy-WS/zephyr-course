

# keep first
board_runner_args(stm32cubeprogrammer "--port=swd" "--reset-mode=hw")
if(CONFIG_XIP AND (CONFIG_STM32_MEMMAP OR CONFIG_BOOTLOADER_MCUBOOT))
  board_runner_args(stm32cubeprogrammer "--extload=MX25UW25645G_NUCLEO-H7S3L8.stldr")
endif()

# stm32 cube programmer is all I need
include(${ZEPHYR_BASE}/boards/common/stm32cubeprogrammer.board.cmake)

