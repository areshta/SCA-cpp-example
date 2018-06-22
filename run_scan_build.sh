#!/bin/bash

scan-build  c++ --std=c++14 -Wimplicit-fallthrough=2 -Wall -Wextra ./source/sca_cpp.cpp &> scan_build.log
less scan_build.log
