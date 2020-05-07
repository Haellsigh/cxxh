# cxxh

cxxh is a command-line tool that helps you create folders, header files, source files and classes for your C++ projects.

## Usage

```
Usage: cxxh [options] <creation option> args
Generic options:
  --help                         prints usage message
  --version                      prints version information
  --verbose                      shows the list of files created
  --dry-run                      do not actually create files
  --force                        overwrite existing files

Creation options:
  -c [ --class ] arg             creates a class
  -h [ --header ] arg            creates a header file
  -s [ --source ] arg            creates a source file

Options:
  -f [ --folder ] arg            creates everything in arg folder
  --include arg                  adds #include <arg> to header or source files
  --lowercase                    creates lowercase filenames, default is true
  --include-guard arg (=pragma)  include guard for header files, one of:
                                 pragma, ifndef
  --header-ext arg (=hh)         header files extension, default is hh
  --source-ext arg (=cpp)        source files extension, default is cpp
```