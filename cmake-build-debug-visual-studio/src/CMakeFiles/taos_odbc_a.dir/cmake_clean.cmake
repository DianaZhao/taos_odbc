file(REMOVE_RECURSE
  "taos_odbc_a.lib"
  "taos_odbc_a.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/taos_odbc_a.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
