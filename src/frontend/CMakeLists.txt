cmake_minimum_required(VERSION 3.9.0)
project(cdcm)

include_directories(
        ../trustwave)

link_directories( ${SMB_LINKS} ${OPENVAS_SMB_LINKS} /usr/lib64/heimdal )

set(SOURCE_FILES
        cdcm_broker.cpp
        message_broker.cpp
        workers_monitor.cpp
		maintenance.cpp)
add_executable(cdcm_broker ${SOURCE_FILES})
#target_link_options(cdcm_broker '-Wl,--enable-new-dtags -Wl,-rpath -Wl,/usr/lib64/heimdal -L/usr/lib64/heimdal -lkrb5'	)

target_link_libraries(cdcm_broker
						pthread 
						zmq
						boost_system
						boost_filesystem
						boost_log
						tw-logger 
						tw-smb-actions 
						tw-common 
						tw-registry-actions  
						tw-internal-actions 
						tw-sessions-cache
						tw-wmi-actions
						tw-zmq 
						tw-singleton-runner
						libpe_pev.a
						openvas_wmiclient
		popt
		libgssapi.a
		asn1
		libkrb5.a
		hcrypto
		heimbase
		heimntlm
		#libhdb.a
		roken
		libhcrypto.a
		dl
		com_err
		glib-2.0
		crypt
		libhx509.a
		libwind.a
		sqlite3
		)
install(TARGETS cdcm_broker
		RUNTIME DESTINATION bin
		DESTINATION ${bin_dest})
install(TARGETS cdcm_broker
		RUNTIME DESTINATION bin
		DESTINATION ${release_bin_dest})