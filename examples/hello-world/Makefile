# name of your application
APPLICATION = Hello_Iot

# If no BOARD is found in the environment, use this default:
BOARD ?= unwd-range-l1-r3

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..
# либы которые юзаются 
USEMODULE += rtctimers-millis
USEMODULE += shell_commands
USEMODULE += shell  
USEMODULE += ps
USEMODULE += opt3001
USEMODULE += xtimer

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1
# пременные для перферии 
FEATURES_REQUIRED += periph_i2c
FEATURES_REQUIRED += periph_rtc
FEATURES_REQUIRED += periph_gpio
FEATURES_REQUIRED += periph_adc

#FEATURES_REQUIRED += xtimer


# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
