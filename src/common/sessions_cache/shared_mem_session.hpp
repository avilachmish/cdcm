//===========================================================================
// Trustwave ltd. @{SRCH}
//								.hpp
//
//---------------------------------------------------------------------------
// DESCRIPTION: @{HDES}
// -----------
//---------------------------------------------------------------------------
// CHANGES LOG: @{HREV}
// -----------
// Revision: 01.00
// By      : Assaf Cohen
// Date    : 15 Jul 2019
// Comments:

#ifndef TRUSTWAVE_MISC__HPP_
#define TRUSTWAVE_MISC__HPP_

#include "../credentials.hpp"

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

namespace trustwave {

    // Typedefs of allocators and containers
    using void_allocator
        = boost::interprocess::allocator<void, boost::interprocess::managed_shared_memory::segment_manager>;
    using CharAllocator
        = boost::interprocess::allocator<char, boost::interprocess::managed_shared_memory::segment_manager>;
    using String = boost::interprocess::basic_string<char, std::char_traits<char>, CharAllocator>;
    //===================================================================
    //                      Forward declarations
    //===================================================================
    class shared_mem_session;
    class shared_mem_session_element;
    class shared_mem_credentials;

    std::ostream& operator<<(std::ostream&, const shared_mem_session&);
    std::ostream& operator<<(std::ostream&, const shared_mem_credentials&);

    class shared_mem_credentials final {
    public:
        ~shared_mem_credentials() = default;
        // Avoid copy semantic
        shared_mem_credentials(const shared_mem_credentials&) = delete;
        shared_mem_credentials& operator=(const shared_mem_credentials&) = delete;

    private:
        friend class shared_mem_session;
        friend struct session_converter;

        friend std::ostream& operator<<(std::ostream&, const shared_mem_credentials&);
        friend std::ostream& operator<<(std::ostream&, const shared_mem_session&);
        explicit shared_mem_credentials(const void_allocator& va);

        // Force move semantic
        shared_mem_credentials(shared_mem_credentials&&) = default;
        shared_mem_credentials& operator=(shared_mem_credentials&&) = default;

        String domain_;
        String username_;
        String password_;
        String workstation_;
    };

    class shared_mem_session final {
    public:
        ~shared_mem_session() = default;
        // Avoid copy semantic
        shared_mem_session(const shared_mem_session&) = delete;
        shared_mem_session& operator=(const shared_mem_session&) = delete;

    private:
        friend struct session_converter;
        friend class shared_mem_sessions_cache;

        friend class shared_mem_session_element;
        friend std::ostream& operator<<(std::ostream&, const shared_mem_session_element&);
        explicit shared_mem_session(const void_allocator& va);

        // Force move semantic
        shared_mem_session(shared_mem_session&&) = default;
        shared_mem_session& operator=(shared_mem_session&&) = default;

        friend std::ostream& operator<<(std::ostream&, const shared_mem_session&);
        String uuid_;
        String remote_;
        shared_mem_credentials creds_;
    };
    // class shared_mem_session

} // namespace trustwave

#endif /* TRUSTWAVE_MISC__HPP_ */
