//=====================================================================================================================
// Trustwave ltd. @{SRCH}
//														read_file.cpp
//
//---------------------------------------------------------------------------------------------------------------------
// DESCRIPTION: 
//
//
//---------------------------------------------------------------------------------------------------------------------
// By      : Assaf Cohen
// Date    : 11/27/19
// Comments:
//=====================================================================================================================
//                          						Include files
//=====================================================================================================================
//=====================================================================================================================
//                          						Include files
//=====================================================================================================================
#include <unistd.h>
#include <string>

#include "../smb_client.hpp"
#include "read_file.hpp"

#include "../../../../common/protocol/msg_types.hpp"
#include "../../../../common/session.hpp"
#include "../../../../common/singleton_runner/authenticated_scan_server.hpp"
using namespace trustwave;
namespace {
    namespace {
        const char base64Alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                      "abcdefghijklmnopqrstuvwxyz"
                                      "0123456789+/";
// A constant defined by Base64 algorithm
        const unsigned char PAD = '=';
    }

    size_t base64_encoded_length(size_t origLen) {
        return (((origLen + 2) / 3) << 2);
    }


    std::string base64_encode(const char *inBuf, ssize_t inLen
                                        ) {
        if (nullptr == inBuf|| 0 == inLen ) {
            AU_LOG_ERROR("inLen : %d inBuf : ",inLen,inBuf);
            return std::string();
        }

        unsigned int bitsContainer = 0;                  // A container for 24 bits from the input stream
        unsigned char currByte; // A current byte from the input stream
        size_t charCount = 0;   // Count byte tripples
        size_t outPos = 0;      // Current letter in the output stream

        std::string ret;
        ret.reserve(base64_encoded_length(inLen));
        while (inLen--) // Scan the input bit stream
        {
            currByte = *(inBuf++);
            bitsContainer |= currByte;
            charCount++;

            if (3 == charCount) {
                ret[outPos++] = base64Alphabet[bitsContainer >> 18 & 0x3f];
                ret[outPos++] = base64Alphabet[(bitsContainer >> 12) & 0x3f];
                ret[outPos++] = base64Alphabet[(bitsContainer >> 6) & 0x3f];
                ret[outPos++] = base64Alphabet[bitsContainer & 0x3f];
                bitsContainer = 0;
                charCount = 0;
            } else { bitsContainer <<= 8; }
        }

        if (charCount) {
            bitsContainer <<= 16 - (8 * charCount);
            ret[outPos++] = base64Alphabet[bitsContainer >> 18 & 0x3f];
            ret[outPos++] = base64Alphabet[(bitsContainer >> 12) & 0x3f];
            if (charCount == 1) { ret[outPos++] = PAD; } else {
                ret[outPos++] = base64Alphabet[(bitsContainer >> 6) & 0x3f];
            }
            ret[outPos++] = PAD;
        }
        //AU_LOG_ERROR("ret : %s",ret.c_str());
        return ret;
    }
}
int SMB_Read_File::act(boost::shared_ptr <session> sess, std::shared_ptr<action_msg> action, std::shared_ptr<result_msg> res)
{
    if (!sess || (sess && sess->id().is_nil())) {
        res->res("Error: Session not found");
        return -1;
    }

    auto smb_action = std::dynamic_pointer_cast<smb_read_file_msg>(action);
    std::string base("smb://");
    base.append(sess->remote()).append("/").append(smb_action->path_);
    std::string tmp_name(authenticated_scan_server::instance().settings.downloaded_files_path_+"/" + sess->idstr() + "-" + action->id());
    trustwave::smb_client rc;
    auto connect_result  = rc.connect(base.c_str());
    if(!connect_result.first)
    {
        res->res(std::string("Error: %s",(connect_result.second == -1?std::string_view ("Unknown error").data():strerror(connect_result.second))));
    }
    auto off = smb_action->offset_.empty()?0:std::stoul(smb_action->offset_);
    auto sz = smb_action->size_.empty()?0:std::stoul(smb_action->size_);
    if (0==sz)
    {
        sz = rc.file_size()-off;
    }
    AU_LOG_ERROR("Recieved offset: %zu size: %zu",off,sz);
    auto buff = new(std::nothrow) char[sz];
    if(nullptr == buff)
    {
        res->res("Error: Memory allocation failed");
        return -1;
    }
    ssize_t r = rc.read(off,sz,buff);
    auto c64_str = base64_encode(buff,r);
    res->res(c64_str.c_str());//fixme assaf figure aou whi strin assignment doesn't  work
    return 0;

}
static std::shared_ptr<SMB_Read_File> instance = nullptr;


// extern function, that declared in "action.hpp", for export the plugin from dll
std::shared_ptr<trustwave::Action_Base> import_action() {
    return instance ? instance : (instance = std::make_shared<SMB_Read_File>());
}