#
# The DAEMONIZE library build rules
#
# Copyright (C) 2015 Samo Pogacnik <samo_pogacnik@t-2.net>
# All rights reserved.
#
# This file is part of the DAEMONIZE software project.
# This file is provided under the terms of the BSD 3-Clause license,
# available in the LICENSE file of the DAEMONIZE software project.
#

# Create a library called "daemonize" which includes the source files "daemonize.c, ...".
# The extension is already found. Any number of sources could be listed here.
add_library (daemonize SHARED daemonize.c) 

set_target_properties (daemonize PROPERTIES VERSION ${daemonize_VERSION_STRING} SOVERSION ${daemonize_VERSION_MAJOR})

install(TARGETS daemonize DESTINATION lib COMPONENT libs)
#install(FILES ${daemonize_SOURCE_DIR}/include/daemonize/libdaemonize.h ${daemonize_SOURCE_DIR}/include/daemonize/log_module.h DESTINATION include/daemonize COMPONENT headers)
