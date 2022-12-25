file(REMOVE_RECURSE
  "taos_odbc.dll"
  "taos_odbc.dll.manifest"
  "taos_odbc.lib"
  "taos_odbc.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/taos_odbc.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
