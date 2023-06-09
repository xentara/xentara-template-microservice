# Microservice Template for Xentara
This repository contains skeleton code for a Xentara microservice that performs signal processing.

## Prerequisites

This skill template requires the Xentara development environment, as well as a Xentara licence. You can get a Xentara
licence in the [Xentara Online Shop](https://www.xentara.io/product/xentara-for-industrial-automation/).

The documentation for Xentara can be found at https://docs.xentara.io/xentara.

This skill template uses the Xentara Utility Library, as well as the Xentara Plugin Framework. The corresponding documentation can be found here:

https://docs.xentara.io/xentara-utils/  
https://docs.xentara.io/xentara-plugin/

## Build Environment

The repository include a [CMakeLists.txt](CMakeLists.txt) file for use with [CMake](https://cmake.org/), which allows you to build the source code
out of the box, as long as the Xentara development environment is installed. If you whish to use a different build system, you must generate the
necessary build configuration file yourself.

## Source Code Documentation

The source code in this repository is documented using [Doxygen](https://doxygen.nl/) comments. If you have Doxygen installed, you can
generate detailed documentation for all classes, functions etc., including a TODO list. To generate the documentation using CMake, just
build the target *docs* by executing the following command in your [build directory](https://cmake.org/cmake/help/latest/manual/cmake.1.html#generate-a-project-buildsystem):

~~~sh
cmake --build . --target docs
~~~

This will generate HTML documentation in the subdirectory *docs/html*.

## Xentara Skill Element Templates

*(See [Skill Elements](https://docs.xentara.io/xentara/xentara_skills.html#xentara_skill_elements) in the [Xentara documentation](https://docs.xentara.io/xentara/))*

### Microservice Instance Template

[src/TemplateInstance.hpp](src/TemplateInstance.hpp)  
[src/TemplateInstance.cpp](src/TemplateInstance.cpp)

The instance skill element template provides template code for services that do signal processing.

The template code has the following features:

- The skill element tracks an error code for the processing it does. If an error occurs, the system is put into a safe state.
- The skill element publishes a [Xentara task](https://docs.xentara.io/xentara/xentara_element_members.html#xentara_tasks) called *execute*
  that performs the signal processing.
- The skill element publishes a [Xentara event](https://docs.xentara.io/xentara/xentara_element_members.html#xentara_events) called *executed*
  that is fired when the microservice is executed.
- The skill element publishes a [Xentara event](https://docs.xentara.io/xentara/xentara_element_members.html#xentara_events) called *error*
  that is fired when an error occurrs.
