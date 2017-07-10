

######################################
# target
######################################
TARGET = libngl

######################################
# building variables
######################################
DEBUG = 0
OPT = -O3

#######################################
# paths
#######################################
# source path
SOURCES_DIR =  \
 \
Bitmaps \
Colors \
Font \
GUI \
GUI/Objects \
HAL_MAL \
HAL_MAL/HX8352 \
HAL_MAL/R61581 \
HAL_MAL/SSD1289 \
Primitives \
TouchScreen

# firmware library path
PERIFLIB_PATH =
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
Bitmaps/bitmap.c \
Colors/colors.c \
Font/Font.c \
GUI/page.c \
GUI/Objects/button.c \
GUI/Objects/checkbox.c \
GUI/Objects/fillbar.c \
GUI/Objects/graphscale.c \
GUI/Objects/greed.c \
GUI/Objects/label.c \
GUI/Objects/seekbar.c \
HAL_MAL/LCD_GPIO.c \
HAL_MAL/LCD_HAL.c \
HAL_MAL/LCD_MAL.c \
HAL_MAL/HX8352/HX8352.c \
HAL_MAL/R61581/R61581.c \
HAL_MAL/SSD1289/SSD1289.c \
Primitives/Clip.c \
Primitives/Graphics_Primitive.c \
Primitives/traingle.c \
TouchScreen/NGL_Touch.c \
TouchScreen/NGL_Touch_Events.c \

# ASM sources
ASM_SOURCES =

######################################
# firmware library
######################################
PERIFLIB_SOURCES =

#######################################
# binaries
#######################################
BINPATH = C:\Program Files\GNU Tools ARM Embedded\5.4 2016q3\bin
PREFIX = arm-none-eabi-
CC = $(BINPATH)/$(PREFIX)gcc
AS = $(BINPATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(BINPATH)/$(PREFIX)objcopy
AR = $(BINPATH)/$(PREFIX)ar
SZ = $(BINPATH)/$(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################

# cpu
CPU = -mcpu=cortex-m4
# fpu
FPU = -mfpu=fpv4-sp-d16
# float-abi
FLOAT-ABI = -mfloat-abi=hard
# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS =

# C defines
C_DEFS =  \
-DARM_MATH_CM4 \
-D__FPU_PRESENT \

# AS includes
AS_INCLUDES =

# C includes
C_INCLUDES =  \
-I. \
-IBitmaps \
-IColors \
-IFont \
-IGUI \
-IHAL_MAL \
-IPrimitives \
-ITouchScreen

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -g -Wall -fdata-sections -ffunction-sections -fno-strict-aliasing

#######################################
# LDFLAGS
#######################################
LDFLAGS = rvs

# default action: build all
all:
	@echo - building $(TARGET)...
	$(MAKE) $(BUILD_DIR)/$(TARGET).a

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $(BUILD_DIR)/$(notdir $(<:.c)) $< -o $@

# $(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
# 	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).a: $(OBJECTS) Makefile
	$(AR) $(LDFLAGS) $@ $(OBJECTS)
	$(SZ) $@

$(BUILD_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	@echo - cleaning $(BUILD_DIR)...
ifeq ($(OS),Windows)
	($(BUILD_DIR):&(rd /s /q "$(BUILD_DIR)" 2> NUL))&
endif
ifeq ($(OS),Linux)
	-@rm -rf $(BUILD_DIR)
	-@rm -rf $(LSTDIR)
	-@rm -rf $(BINDIR)
endif

#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***