#
# The demo programs build rules
#
# Copyright (C) 2014 Samo Pogacnik <samo_pogacnik@t-2.net>
# All rights reserved.
#
# This file is part of the DAEMONIZE software project.
# This file is provided under the terms of the BSD 3-Clause license,
# available in the LICENSE file of the EVM software project.
#

# Add executable called "hello_daemonize" that is built from the source files  
# "hello_daemonize.c" and "hello_daemonize.h".  
add_executable (hello_daemonize hello_daemonize.c hello_daemonize.h)    
        
# Make sure the linker can find the Hello library once it is built.  
link_directories (${daemonize_BINARY_DIR}/daemonize)

# Link the executable to the evm library.  
target_link_libraries (hello_daemonize daemonize)

install(TARGETS hello_daemonize RUNTIME DESTINATION bin COMPONENT demos)

