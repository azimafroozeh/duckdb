add_library(
        avx2
        OBJECT
        add.cpp
        arithmetic.cpp
        bitwise.cpp
        multiply.cpp
        subtract.cpp
        operators.cpp)

target_compile_definitions(avx2 PRIVATE AVX2)
set_target_properties (avx2 PROPERTIES COMPILE_FLAGS "-O3 -mavx512f")


LIST(APPEND ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:avx2>)

add_library(
  duckdb_func_ops
  OBJECT
  add.cpp
  arithmetic.cpp
  bitwise.cpp
  multiply.cpp
  subtract.cpp
        operators.cpp)

target_compile_definitions(duckdb_func_ops PRIVATE SCALAR)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:duckdb_func_ops>
        PARENT_SCOPE)


#add_library_unity(
#        avx2
#        OBJECT
#        add.cpp
#        arithmetic.cpp
#        bitwise.cpp
#        multiply.cpp
#        subtract.cpp)
#
#set_target_properties (avx2 PROPERTIES COMPILE_FLAGS "-O3 -mavx2")

#get_property(a TARGET avx2 PROPERTY COMPILE_FLAGS)
#message("${a}")










