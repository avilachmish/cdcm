
set (OPENVAS_SMB_SRC ${DEPS_DIR}/openvas-smb) #/sandbox/src/deps/
set (OPENVAS_SMB_INCLUDES
        ${OPENVAS_SMB_SRC}
        ${OPENVAS_SMB_SRC}/include
        )

set (OPENVAS_SMB_LINKS
        ${OPENVAS_SMB_SRC}/build/wmi
        )

SET (OPENVAS_SMB_LIBS_TO_LINK
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

