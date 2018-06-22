#!/bin/bash
~/cppcheck-1.84/cppcheck --enable=all --template="{file}:{line}: {severity}: {message}\n{code}" ./source &> cppcheck.log
less cppcheck.log
