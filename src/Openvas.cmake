
set (OPENVAS_SMB_SRC ${DEPS_DIR}/openvas-smb) #/sandbox/src/deps/
set (OPENVAS_SMB_INCLUDES
        ${OPENVAS_SMB_SRC}
        #${OPENVAS_SMB_SRC}/samba/include
        #${OPENVAS_SMB_SRC}/wmi
        )

set (OPENVAS_SMB_LINKS
        /usr/local/lib
        ${OPENVAS_SMB_SRC}/wmi
        )

