set(arm_srcs "src/arm.c")

idf_component_register(SRCS "${arm_srcs}"
                    INCLUDE_DIRS "include")