set( APP_NAME  filter_tests )

## If XCORE_TARGET hasn't been specified, default to XCORE-AI-EXPLORER
if( NOT DEFINED XCORE_TARGET )
  set( XCORE_TARGET XCORE-AI-EXPLORER )
endif()


## Sources
file( GLOB_RECURSE SOURCES_C  src/*.c )
file( GLOB_RECURSE SOURCES_XC src/*.xc )

add_executable( ${APP_NAME} )

target_sources( ${APP_NAME}
  PRIVATE
    ${SOURCES_C}
    $<$<PLATFORM_ID:XCORE_XS3A>:${SOURCES_XC}>
)

target_include_directories( ${APP_NAME} 
  PRIVATE 
    src 
)

target_compile_options( ${APP_NAME}
  PRIVATE 
    -Os -g -Wno-format -Wno-unused-variable
    $<$<PLATFORM_ID:XCORE_XS3A>:-MMD>
    $<$<PLATFORM_ID:XCORE_XS3A>:-fxscope>
    $<$<PLATFORM_ID:XCORE_XS3A>:-mcmodel=large>
    $<$<PLATFORM_ID:XCORE_XS3A>:-Wno-xcore-fptrgroup>
    # $<$<PLATFORM_ID:Linux>:
)

target_link_options( ${APP_NAME}
  PRIVATE
    $<$<PLATFORM_ID:XCORE_XS3A>:-target=${XCORE_TARGET}>
    $<$<PLATFORM_ID:XCORE_XS3A>:-report>
    $<$<PLATFORM_ID:XCORE_XS3A>:-fcmdline-buffer-bytes=1024>
    $<$<PLATFORM_ID:XCORE_XS3A>:-fxscope>
)

target_compile_definitions( ${APP_NAME}
  PRIVATE
    $<$<PLATFORM_ID:Linux>:USE_REF=1>
    SMOKE_TEST=$<BOOL:${XS3_MATH_SMOKE_TEST}>
)

target_link_libraries( ${APP_NAME}
  lib_xs3_math
  test_shared
  unity
  $<$<PLATFORM_ID:Linux>:m>
)

add_dependencies( lib_xs3_math-tests  ${APP_NAME} )

#########


# if ( ${Python3_FOUND} )

#   # Run the python scripts for filter generation. This is just to ensure that
#   # the script runs correctly and produces code that compiles correctly.

#   set( SCRIPT_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/../../lib_xs3_math/script    )
#   set( GEN_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/src/gen.local                )
#   set( COEF_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/etc                          )

#   target_sources( ${APP_NAME}
#     PRIVATE
#       ${CMAKE_CURRENT_SOURCE_DIR}/src/gen.local/test_fir_s32.c
#       ${CMAKE_CURRENT_SOURCE_DIR}/src/gen.local/test_fir_s16.c
#       ${CMAKE_CURRENT_SOURCE_DIR}/src/gen.local/test_biquad_s32.c 
#   )

#   add_custom_command(
#     OUTPUT ${GEN_DIR}/test_fir_s32.h ${GEN_DIR}/test_fir_s32.c
#     COMMAND ${Python3_EXECUTABLE} ${SCRIPT_DIR}/gen_fir_filter_s32.py test_fir_s32 --out-dir ${GEN_DIR} ${COEF_DIR}/coefs.txt
#     DEPENDS ${COEF_DIR}/coefs.txt
#     COMMENT "Generating test_fir_s32 from script" )

#   add_custom_command(
#     OUTPUT ${GEN_DIR}/test_fir_s16.h ${GEN_DIR}/test_fir_s16.c
#     COMMAND ${Python3_EXECUTABLE} ${SCRIPT_DIR}/gen_fir_filter_s16.py test_fir_s16 --out-dir ${GEN_DIR} ${COEF_DIR}/coefs.txt
#     DEPENDS ${COEF_DIR}/coefs.txt
#     COMMENT "Generating test_fir_s16 from script" )

#   add_custom_command(
#     OUTPUT ${GEN_DIR}/test_biquad_s32.h ${GEN_DIR}/test_biquad_s32.c
#     COMMAND ${Python3_EXECUTABLE} ${SCRIPT_DIR}/gen_biquad_filter_s32.py test_biquad_s32 --out-dir ${GEN_DIR} ${COEF_DIR}/biquad_coefs.csv
#     DEPENDS ${COEF_DIR}/coefs.txt
#     COMMENT "Generating test_biquad_s32 from script" )

# endif()
#########
