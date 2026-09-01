foreach(required_variable IN ITEMS CONVERTER INPUT OUTPUT SYMBOL)
  if(NOT DEFINED ${required_variable})
    message(FATAL_ERROR "${required_variable} is required")
  endif()
endforeach()

execute_process(
  COMMAND "${CONVERTER}" -u8 "${INPUT}" "${SYMBOL}"
  OUTPUT_FILE "${OUTPUT}"
  ERROR_VARIABLE converter_error
  RESULT_VARIABLE converter_result
)

if(NOT converter_result EQUAL 0)
  message(FATAL_ERROR "Failed to embed ${INPUT}: ${converter_error}")
endif()
