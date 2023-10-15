# Custom matrix configuration (if enabled)
ifeq ($(strip $(CUSTOM_MATRIX)), lite)
SRC += uconsole_matrix.c
endif

# Trackball configuration (if enabled)
ifeq ($(strip $(TRACKBALL_ENABLE)), yes)
    POINTING_DEVICE_ENABLE := yes
    POINTING_DEVICE_DRIVER := custom
    OPT_DEFS               += -DTRACKBALL_ENABLE
    SRC                    += trackball.c
endif

# # enable matrix scanrate debug if console is enabled
# ifeq ($(strip $(CONSOLE_ENABLE)), yes)
#     DEBUG_MATRIX_SCAN_RATE_ENABLE := yes
# endif
