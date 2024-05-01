## Modifications for Mac Apple Silicon

The following modifications were made to the project to ensure compatibility with Mac Apple Silicon:

1. `catch.hpp`:
- The `catch.hpp` file was updated to a version 2.13.4 [here](https://github.com/catchorg/Catch2/blob/v2.13.4/single_include/catch2/catch.hpp).
- The following code snippet was added due to macOS Sonoma [issues](https://github.com/catchorg/Catch2/issues/2837):
  ```cpp
  #ifndef __has_extension
  #define __has_extension(x) 0
  #endif
  ```

2. `Makefile`:
- Removed `-msse2` flag.
