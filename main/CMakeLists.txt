set(srcs "ESP32APA102Driver.c" "apa102LEDStrip.c" "colourObject.c")

idf_component_register(SRCS ${srcs} INCLUDE_DIRS ".")
