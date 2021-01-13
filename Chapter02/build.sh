#!/bin/bash

################################################
# If you have a bash shell you can run this
# script.  If not you can copy and paste the 
# line below to the command line.
################################################

emcc shell.c -o shell-test.html --shell-file new_shell.html -s NO_EXIT_RUNTIME=1 -s EXPORTED_FUNCTIONS="['_test', '_string_test', '_int_test', '_float_test', '_main']" -s EXTRA_EXPORTED_RUNTIME_METHODS="['cwrap', 'ccall']"

# There were multiple builds in this chapter

emcc canvas.c -o canvas.html --shell-file canvas_shell.html