# --- Set option if unilitialized yet
function (set_option option_name option_value)
  if (NOT ${option_name}) 
    set (${option_name} ${option_value} CACHE INTERNAL "")
  endif ()
endfunction ()
