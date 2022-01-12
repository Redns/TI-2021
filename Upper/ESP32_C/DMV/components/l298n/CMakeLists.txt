set(l298n_srcs "src/l298n.c")

idf_component_register(SRCS "${l298n_srcs}"
                    INCLUDE_DIRS "include")