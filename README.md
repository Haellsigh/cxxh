# cxxh

cxxh is a command-line tool that helps you create folders, header files, source files and classes for your C++ projects.

## Getting started for development

In the root folder
```
.\vcpkg\bootstrap-vcpkg.bat -disableMetrics
.\vcpkg\vcpkg install cxxopts fmt boost-program-options boost-locale ms-gsl --triplet=x64-windows-static
```

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

## Features

- Creates files using a common, easy to learn identification pattern:
  > `cxxh -c Multiple/Folder/Any::Namespace::ClassName`

  will create a ClassName class in the files classname.hh and classname.cpp in the folder Multiple/Folder and within the namespace Any::Namespaces.
- Creates headers, source or classes.
- Adds multiple `#include <>`
- Prevents overwriting files by default

## Planned features

- Automatically add files to CMakeLists.txt
