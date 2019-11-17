
set (OPENVAS_SMB_SRC ${DEPS_DIR}/openvas-smb) #/sandbox/src/deps/
set (OPENVAS_SMB_INCLUDES
        ${OPENVAS_SMB_SRC}
        ${OPENVAS_SMB_SRC}/include
        )

set (OPENVAS_SMB_LINKS
        ${OPENVAS_SMB_SRC}/build/wmi
        )

