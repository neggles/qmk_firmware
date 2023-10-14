# enable custom matrix scanning
CUSTOM_MATRIX = lite
SRC += uconsole_matrix.c

# enable trackball
TRACKBALL_ENABLE = no

ifeq ($(strip $(TRACKBALL_ENABLE)), yes)
    POINTING_DEVICE_ENABLE := yes
    POINTING_DEVICE_DRIVER := custom
    OPT_DEFS += -DTRACKBALL_ENABLE
    SRC += trackball.c
endif
