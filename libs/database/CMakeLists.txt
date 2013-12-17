project(database)

include(build_functions)

add_standard_lib(
  common
)

use_sqlite3()