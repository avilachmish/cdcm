
set (OPENVAS_SMB_SRC ${DEPS_DIR}/openvas-smb) #/sandbox/src/deps/
message(${DEPS_DIR})
set (OPENVAS_SMB_INCLUDES
        ${OPENVAS_SMB_SRC}
       # ${OPENVAS_SMB_SRC}/wmi
        )
message(${OPENVAS_SMB_INCLUDES})
set (OPENVAS_SMB_LINKS
        ${OPENVAS_SMB_SRC}/build/wmi
        /usr/lib64/heimdal
        )

SET (OPENVAS_SMB_LIBS
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

link_directories(${OPENVAS_SMB_LINKS} )
include_directories(${OPENVAS_SMB_INCLUDES})

